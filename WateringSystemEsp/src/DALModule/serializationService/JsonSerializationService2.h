/*
 * SerializationService2.h
 *
 *  Created on: 8 áôáø× 2018
 *      Author: rina
 */

#ifndef SERIALIZATIONSERVICE2_H_
#define SERIALIZATIONSERVICE2_H_

#include <iostream>
#include <vector>
#include <ArduinoJson.h>


namespace DAL {

//-------------------------------------------- detail input archive
enum class sfinae {};

template<typename, typename T>
struct has_load_method {
    static_assert(
        std::integral_constant<T, false>::value,
        "has_load_method failed : Second template parameter needs to be of function type.");
};

template<typename C, typename Ret, typename... Args>
struct has_load_method<C, Ret(Args...)> {
private:
    template<typename T>
    static constexpr auto check(T*)-> typename std::is_same< decltype( std::declval<T>().load( std::declval<Args>()... ) ), Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
																																	>::type;  // attempt to call it and see if the return type is correct

    template<typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

//--------------------------------------------

//-------------------------------------------- detail output archive
template <typename T>
struct has_save_method
{
    struct dummy { /* something */ };

    template <typename C, typename P>
    static auto test(P * p) -> decltype(std::declval<C>().save(*p), std::true_type());

    template <typename, typename>
    static std::false_type test(...);

    typedef decltype(test<T, dummy>(nullptr)) type;
    static const bool value = std::is_same<std::true_type, decltype(test<T, dummy>(nullptr))>::value;
};
//--------------------------------------------

//--------------------------------------------
struct NameValuePairCore {};

template <class T>
class NameValuePair : NameValuePairCore
 {
   private:
     // If we get passed an array, keep the type as is, otherwise store
     // a reference if we were passed an l value reference, else copy the value
     using Type = typename std::conditional<std::is_array<typename std::remove_reference<T>::type>::value,
                                            typename std::remove_cv<T>::type,
                                            typename std::conditional<std::is_lvalue_reference<T>::value,
                                                                      T,
                                                                      typename std::decay<T>::type>::type>::type;

     // prevent nested nvps
     static_assert( !std::is_base_of<NameValuePairCore, T>::value,
                    "Cannot pair a name to a NameValuePair" );

     NameValuePair & operator=( NameValuePair const & ) = delete;

   public:
     //! Constructs a new NameValuePair
     /*! @param n The name of the pair
         @param v The value to pair.  Ideally this should be an l-value reference so that
                  the value can be both loaded and saved to.  If you pass an r-value reference,
                  the NameValuePair will store a copy of it instead of a reference.  Thus you should
                  only pass r-values in cases where this makes sense, such as the result of some
                  size() call.
         @internal */
     NameValuePair(std::string n, T && v ) : name(n), value(std::forward<T>(v)) {}

     std::string name;
     Type value;
 };

template<class T> inline
NameValuePair<T> make_nvp(std::string name, T&& value){
	return {name, std::forward<T>(value)};
}
//--------------------------------------------

class DoNothingMappingFile {
public:
	template<typename ModelType, typename JsonObjType>
	void Model2Json(ModelType&, JsonObjType&) {

	}
};

//giving the same interface to jsonArray/Object for more readable code
class JsonContex {
	bool _isJsonObj;
public:
	JsonObject* _jsonObj;
	JsonArray* _jsonArr;
	std::string nextName;

	JsonContex(JsonObject* jsonObj): _isJsonObj(true), _jsonObj(jsonObj), _jsonArr(NULL) {}
	JsonContex(JsonArray* jsonArr): _isJsonObj(false), _jsonObj(NULL), _jsonArr(jsonArr) {}
	JsonContex(JsonContex&& other) = default;

	bool isJsonObject(){
		return _isJsonObj;
	}

	JsonContex&& createNestedObject() {
		if(_isJsonObj)
			return JsonContex(&(_jsonObj->createNestedObject(nextName)));
		JsonObject& jObj = _jsonArr->createNestedObject();
		return JsonContex(&jObj);
	}

	JsonContex&& createNestedArray() {
		if(_isJsonObj)
			return &_jsonObj->createNestedArray(nextName);
		return &_jsonArr->createNestedArray();
	}

	//revisit the reference stuff
	template<typename T>
	bool add(T&& t){
		if(_isJsonObj)
			return _jsonObj->set(nextName, std::forward(t));
		return _jsonArr->add(std::forward(t));
	}

};

class OutputArchiveBase {};

template<typename MappingFile>
class SerializationService2 : OutputArchiveBase {
public:
	MappingFile _mappingFile;
	StaticJsonBuffer<5000> jsonBuffer;
	std::vector<JsonContex> contexList;

	SerializationService2(MappingFile mappingFile):
		_mappingFile(mappingFile){}
	virtual ~SerializationService2(){}

	//! Serializes all passed in data
	/*! This is the primary interface for serializing data with an archive */
	template<class ... Types>
	void operator()(Types && ... args) {
		addProperties(std::forward<Types>( args )...);//add the properties of the object (the current one we are filling his json in)
	}

	//catching all the NameValuePairs. we save its name as the nextName for the json context
	/*template<class T>
	void addProperty( NameValuePair<T> && head) {
		contexList.back().nextName = head.name;//save the name
		addProperty(head.value);//continue
	}*/

	//! property with save method - the property is an object in json and we are in object context
	template<class T, typename std::enable_if<has_save_method<T>::value ,  sfinae>::type = {}>
	void addProperty( NameValuePair<T> && head) {
		//getting the correct contex
		auto& contex = contexList.back();
		if(!contex.isJsonObject())
			std::cout << "ERROR: contex suppose to be jsonObject (in enable_if<has_save_method<T>)" << std::endl;
		JsonObject& jsonObj = *(contex._jsonObj);

		//checking if the user manually entered this property (by checking if the key exists)
		if(jsonObj.containsKey(head.name))
			return;

		//adding this property to the current contex
		JsonObject& propContex = jsonObj.createNestedObject(head.name);
		_mappingFile.Model2Json(head.value, propContex);
		contexList.push_back(&propContex);
		head.value.save(*this);//adding this property properties (basically like calling 'addProperties' with all the properties the user want)
		contexList.pop_back();//must to remember to pot the context after we done with it (do some auto pop context thing on scope destruction..)
	}

	//! property with save method - the property is an object, we didn't get NameValuePair so we are inside array
	template<class T, typename std::enable_if<has_save_method<T>::value ,  sfinae>::type = {}>
	void addProperty(T && t) {
		//getting the correct contex
		auto& contex = contexList.back();
		if(contex.isJsonObject())
			std::cout << "ERROR: contex suppose to be jsonArray (in std::enable_if<has_save_method<T>)" << std::endl;
		JsonArray& jsonArr = *(contex._jsonArr);

		//adding this property to the current contex
		JsonObject& propContex = jsonArr.createNestedObject();
		contexList.push_back(&propContex);
		_mappingFile.Model2Json(t, propContex);
		t.save(*this);//adding this property properties (basically like calling 'addProperties' with all the properties the user want)
		contexList.pop_back();//must to remember to pot the context after we done with it (do some auto pop context thing on scope destruction..)
	}

	//int/string/double/bool properties; we are nameValuePair so we are inside an object contex
	template<class T, typename std::enable_if<std::is_arithmetic<typename std::remove_reference<T>::type>::value || std::is_same<typename std::remove_reference<T>::type, std::string>::value   , sfinae>::type = {}>
	void addProperty(NameValuePair<T> && head) {
		//getting the correct contex
		auto& contex = contexList.back();
		if(!contex.isJsonObject())
			std::cout << "ERROR: contex suppose to be jsonObject (in is_arithmetic<typename )" << std::endl;
		JsonObject& jsonObj = *(contex._jsonObj);

		//checking if the user manually entered this property (by checking if the key exists)
		if(jsonObj.containsKey(head.name))
			return;

		//adding this property to the current contex
		if(!jsonObj.set(head.name, head.value))
			std::cout << "ERROR: failed to set a key to jsonObject" << std::endl;
	}

	//int/string/double/bool properties; we not nameValuePair so we are inside an array
	template<class T, typename std::enable_if<std::is_arithmetic<typename std::remove_reference<T>::type>::value || std::is_same<typename std::remove_reference<T>::type, std::string>::value   , sfinae>::type = {}>
	void addProperty(T && t) {
		//getting the correct contex (array)
		auto& contex = contexList.back();
		if(contex.isJsonObject())
			std::cout << "ERROR: contex suppose to be jsonArr (in enable_if<std::is_arithmetic)" << std::endl;
		JsonArray& jsonArray = *(contex._jsonArr);

		//adding this property to the current contex
		if(!jsonArray.add(t))
			std::cout << "ERROR: failed to set a key to jsonArr" << std::endl;
	}


	//NameValuePair - means that we are in objJson contex, vector mean that the property is an array
	template<class T, typename A>
	void addProperty(NameValuePair<std::vector<T, A>& > && head) {

		//getting the correct contex
		auto& contex = contexList.back();
		if(!contex.isJsonObject())
			std::cout << "ERROR: contex suppose to be jsonObject (in NameValuePair<std::vector)" << std::endl;
		JsonObject& jsonObj = *(contex._jsonObj);

		//checking if the user manually entered this property (by checking if the key exists)
		if(jsonObj.containsKey(head.name))
			return;

		//creating new array contex
		JsonArray& propJsonArrCntx = jsonObj.createNestedArray(head.name);
		contexList.push_back(&propJsonArrCntx);

		//adding all the array items
		for(auto& t : head.value){
			addProperty(t);
		}

		contexList.pop_back();
	}

	//! Unwinds to process all data
	template <class T, class ... Other> inline
	void addProperties( T && head, Other && ... tail )
	{
		addProperty( std::forward<T>( head ) );
		addProperties( std::forward<Other>( tail )... );
	}

	//! Unwinds to process all data

	void addProperties()
	{

	}


	template<typename ModelType>
	std::string Model2Json(ModelType& model){
		// creating the json contex
		JsonObject& root = jsonBuffer.createObject();
		contexList.push_back(&root);

		//starting the serialization of model by serializing his properties
		_mappingFile.Model2Json(model, root);//checking to see if the user want to override our default implementation (buy mannualy adding some keys)

		//calling to start to serialize the model properties (adding the keys our user didn't add)
		model.save(*this);
		//for(auto property : properties)
		//	if(!root.containsKey(property.key)){//serializing the property only if the key for that property doesn't already exists (if it is it meaning the user already costume defined it)
		//		addProperty(model, property, root);
		//	}
		//return "Not implemented yet";//root.toString()
		char buffer[400];
		root.printTo(buffer, sizeof(buffer));
		return std::string(buffer);
	}


};

//------------------------------------------- temporary for development
class A {
public:
	A(){}
	A(int aa): a(aa){}
	int a = 1;
	std::string sa = "kibata";
	std::vector<int> arr = {1,2,3};

	template<class Archive>
	void save( Archive& archive) {
		archive(DAL::make_nvp("a", a), DAL::make_nvp("sa", sa), DAL::make_nvp("arr", arr));
	}
};

class B {
public:
	A ca;
	int b = 2;
	std::vector<int> brr = {1,2,5};
	std::vector<A> acrr = {A(7),A(8),A(4)};

	template<class Archive>
	void save( Archive& archive) {
		archive(DAL::make_nvp("ca", ca), DAL::make_nvp("b", b), DAL::make_nvp("brr", brr), DAL::make_nvp("acrr", acrr));
	}
};

void run(){
	B b;
	A a;

	DAL::SerializationService2<DAL::DoNothingMappingFile> *serSevice = new DAL::SerializationService2<DAL::DoNothingMappingFile>(DAL::DoNothingMappingFile());
	std::cout << "the json string is: " << std::endl;
	std::string generatedJson = serSevice->Model2Json(b);
	std::cout << generatedJson << std::endl;

}

//------------------------------------------------


} /* namespace DAL */

#endif /* SERIALIZATIONSERVICE2_H_ */
