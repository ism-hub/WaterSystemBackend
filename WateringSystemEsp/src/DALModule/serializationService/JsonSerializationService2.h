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
#include <WString.h>
#include <ObserverDesignPattern/Property.hpp>
//#include <Sprinkler.h>

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
     NameValuePair(String n, T && v ) : name(n), value(std::forward<T>(v)) {}

     String name;
     Type value;
 };

template<class T> inline
NameValuePair<T> make_nvp(String name, T&& value){
	return {name, std::forward<T>(value)};
}

#define MACRO_NVP(T) ::DAL::make_nvp(#T, T)
//--------------------------------------------

class DoNothingMappingFile {
public:
	template<typename ModelType, typename JsonObjType>
	void Model2Json(const ModelType&, JsonObjType&) {

	}
};

//giving the same interface to jsonArray/Object for more readable code
class JsonContex {
	bool _isJsonObj;
public:
	JsonObject* _jsonObj;
	JsonArray* _jsonArr;
	const String* nextName ;

	JsonContex(JsonObject* jsonObj): _isJsonObj(true), _jsonObj(jsonObj), _jsonArr(NULL), nextName(NULL) {}
	JsonContex(JsonArray* jsonArr): _isJsonObj(false), _jsonObj(NULL), _jsonArr(jsonArr), nextName(NULL) {}
	//JsonContex(JsonContex&& other) = default;
	//JsonContex(JsonContex& other) = default;

	bool isJsonObject(){
		return _isJsonObj;
	}

	JsonContex createNestedObject() {
		if(_isJsonObj)
			return JsonContex(&(_jsonObj->createNestedObject(*nextName)));
		return JsonContex(&_jsonArr->createNestedObject());
	}

	JsonContex createNestedArray() {
		if(_isJsonObj)
			return &_jsonObj->createNestedArray(*nextName);
		return &_jsonArr->createNestedArray();
	}

	//revisit the reference stuff
	template<typename T>
	bool add(const T& t){
		if(_isJsonObj)
			return _jsonObj->set(*nextName, t);
		return _jsonArr->add(t);
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
	//template<class ... Types>
	//void operator()(Types && ... args) {
	//	addProperties(std::forward<Types>( args )...);//add the properties of the object (the current one we are filling his json in)
	//}

	//catching all the NameValuePairs. we save its name as the nextName for the json context
	template<class T>
	void addProperty(const NameValuePair<T>& head){
		contexList.back().nextName = &head.name;//save the name
		addProperty(head.value);//continue
	}

	//! property with save method - the property is an object, we didn't get NameValuePair so we are inside array
	template<class T, typename std::enable_if<has_save_method<T>::value ,  sfinae>::type = {}>
	void addProperty(const T& t) {
		//getting the correct contex
		auto& contex = contexList.back();

		//adding this property to the current contex
		JsonContex propContex = contex.createNestedObject();//add here a destructor that pops the element from the list by itself?
		contexList.push_back(propContex);
		_mappingFile.Model2Json(t, propContex);
		t.save(*this);//adding this property properties (basically like calling 'addProperties' with all the properties the user want)
		contexList.pop_back();//must to remember to pot the context after we done with it (do some auto pop context thing on scope destruction..)
	}

	//int/string/double/bool properties; we not nameValuePair so we are inside an array
	template<class T, typename std::enable_if<std::is_arithmetic<typename std::remove_reference<T>::type>::value || std::is_same<typename std::remove_reference<T>::type, String>::value   , sfinae>::type = {}>
	void addProperty(const T& t) {
		auto& contex = contexList.back();

		//adding this property to the current contex
		if(!contex.add(t))
			std::cout << "ERROR: failed to set a key in enable_if<std::is_arithmetic" << std::endl;
	}


	//NameValuePair - means that we are in objJson contex, vector mean that the property is an array
	template<class T, typename A>
	void addProperty(std::vector<T, A> const &  vec) {

		//getting the correct contex
		auto& contex = contexList.back();

		//creating new array contex
		JsonContex propJsonArrCntx = contex.createNestedArray();
		contexList.push_back(propJsonArrCntx);

		//adding all the array items
		for(const auto& t : vec){
			addProperty(t);
		}

		contexList.pop_back();
	}

	//handle property type (invisible)
	template <typename T>
	void addProperty(Model::Property<T> const & prop) {
		addProperty(prop.get());
	}

	//! Unwinds to process all data
	template <class T, class ... Other> inline
	void addProperties(const T& head, const Other& ... tail )
	{
		addProperty(head );
		addProperties( tail ... );
	}
	void addProperties(){}


	template<typename ModelType>
	String Model2Json(const ModelType& model){//const reference can take rvalues
		// creating the json contex
		JsonObject& root = jsonBuffer.createObject();
		contexList.push_back(&root);

		//starting the serialization of model by serializing his properties
		_mappingFile.Model2Json(model, root);//checking to see if the user want to override our default implementation (buy mannualy adding some keys)

		//calling to start to serialize the model properties (adding the keys our user didn't add)
		model.save(*this);

		char buffer[400];
		root.printTo(buffer, sizeof(buffer));
		return String(buffer);
	}

};

//------------------------------------------- temporary for development
class A {
public:
	A() {
		std::cout << "A CTOR" << std::endl;
	}

	A(const A& a) {
		std::cout << "const A COPY CTOR" << std::endl;
	}
	A(A&& a) {
		std::cout << "A MOVE CTOR" << std::endl;
	}
	A(const A&& a) {
		std::cout << "const A MOVE CTOR" << std::endl;
	}
	~A(){
		std::cout << "A DTOR" << std::endl;
	}
	A(int aa): a(aa){
		std::cout << "A CTOR 2" << std::endl;
	}

	int a = 1;
	String sa = "kibata";
	std::vector<int> arr = {1,2,3};

	template<class Archive>
	void save( Archive& archive) const{
		//archive.addProperties(DAL::make_nvp("a", a), DAL::make_nvp("sa", sa), DAL::make_nvp("arr", arr));
		archive.addProperties(DAL::make_nvp("a", a));
	}
};

class B {
public:

	A ca;
	int b = 2;
	std::vector<int> brr = {1,2,5};
	std::vector<A> acrr = {A(7),A(8),A(4)};

	template<class Archive>
	void save( Archive& archive) const {
		//archive.addProperties(DAL::make_nvp("ca", ca), DAL::make_nvp("b", b), DAL::make_nvp("brr", brr), DAL::make_nvp("acrr", acrr));
		archive.addProperties(DAL::make_nvp("b", b));
	}
};

void run(){
	//B b;
	A a;


	DAL::SerializationService2<DAL::DoNothingMappingFile> *serSevice = new DAL::SerializationService2<DAL::DoNothingMappingFile>(DAL::DoNothingMappingFile());
	std::cout << "the json string is: " << std::endl;
	String generatedJson = serSevice->Model2Json(a);
	std::cout << generatedJson << std::endl;

}

//------------------------------------------------


} /* namespace DAL */

#endif /* SERIALIZATIONSERVICE2_H_ */
