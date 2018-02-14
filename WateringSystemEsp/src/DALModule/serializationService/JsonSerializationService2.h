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
#include <algorithm>
//#include <Sprinkler.h>

namespace DAL {

//-------------------------------------------- detail input archive
enum class sfinae {};

template <typename T>
struct has_load_method
{
    struct dummy { /* something */ };

    template <typename C, typename P>
    static auto test(P * p) -> decltype(std::declval<C>().load(*p), std::true_type());

    template <typename, typename>
    static std::false_type test(...);

    typedef decltype(test<T, dummy>(nullptr)) type;
    static const bool value = std::is_same<std::true_type, decltype(test<T, dummy>(nullptr))>::value;
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



//giving the same interface to jsonArray/Object for more readable code
class JsonContex {
	bool _isJsonObj;
public:
	JsonObject* _jsonObj;
	JsonArray* _jsonArr;
	const String* nextName ;//next Key
	std::vector<String> listOfNotAllowedKeys;// list of names that we not aloowed as keys in this context,

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

	template<typename TString>
	bool containsKey(const TString& key){
		if(!_isJsonObj)
			return false;
		return _jsonObj->containsKey(key);
	}

	int getKeyValue(){//TODO: implement properly
		if(_isJsonObj)
			return (*_jsonObj)[*nextName];
		return -1;
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

	//catching all the NameValuePairs. we save its name as the nextName for the json context
	//if we handle a NameValuePair it means that we inside an object and we want to add to it a property named head.name
	//if a property names like that already exists we dont cont (the mapping file already costume added it)
	template<class T>
	void addProperty(const NameValuePair<T>& head){
		auto& contex = contexList.back();
		contex.nextName = &head.name;//save the name

		const std::vector<String>& bannedKeys = contex.listOfNotAllowedKeys;
		if(contex.containsKey(head.name) || std::find(bannedKeys.begin(), bannedKeys.end(), head.name) != bannedKeys.end())//if we already have this key we dont handle this object
			return;

		addProperty(head.value);//continue
	}

	//! property with save method - the property is an object, we didn't get NameValuePair so we are inside array
	template<class T, typename std::enable_if<has_save_method<T>::value ,  sfinae>::type = {}>
	void addProperty(const T& t) {
		//getting the correct contex
		auto& contex = contexList.back();

		//adding this property to the current contex
		//JsonContex propContex = contex.createNestedObject();//add here a destructor that pops the element from the list by itself?
		contexList.push_back(contex.createNestedObject());
		_mappingFile.Model2Json(t, contexList.back(), *this);
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

	//handle shared pointer type (invisible)
	template <typename T>
	void addProperty(const std::shared_ptr<T>& sharedPtr) {
		if(sharedPtr != nullptr)
			addProperty(*sharedPtr);
		else{// if we null ptr we need to add null property
			if(!contexList.back().add((char*)0))
				std::cout << "ERROR: failed to set null value" << std::endl;
		}
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
		_mappingFile.Model2Json(model, root, *this);//checking to see if the user want to override our default implementation (buy mannualy adding some keys)

		//calling to start to serialize the model properties (adding the keys our user didn't add)
		model.save(*this);

		char buffer[400];
		root.printTo(buffer, sizeof(buffer));

		//cleanup
		contexList.clear();
		jsonBuffer.clear();

		return String(buffer);
	}

	//loading only the keys that are in the json if no key is present we just ignore the property, even if it has nameValuePair
	template<typename ModelType>
	void Json2Model(ModelType& model, const String& json){//we are taking references and fill them
		JsonObject& root = jsonBuffer.parseObject(json);
		contexList.push_back(&root);

		//starting the serialization of model by serializing his properties
		_mappingFile.Json2Model(model, root, *this);//checking to see if the user want to override our default implementation (he can add keys to the banned keys list of the contex and we will ignore them when loading)

		//calling to start to serialize the model properties (adding the keys our user didn't add)
		model.load(*this);

		//cleanup
		contexList.clear();
		jsonBuffer.clear();
	}

	//! Unwinds to process all data
	template <class T, class ... Other> inline
	void loadProperties(T&& head, Other&& ... tail )
	{
		loadProperty(head );//I think head is lvalue now (usually T&& is rvalue when we get NameValuePair)
		loadProperties( tail ... );
	}
	void loadProperties(){}

	template<class T>
	void loadProperty(NameValuePair<T>& head){
		auto& contex = contexList.back();
		contex.nextName = &head.name;//save the name

		const std::vector<String>& bannedKeys = contex.listOfNotAllowedKeys;
		if(std::find(bannedKeys.begin(), bannedKeys.end(), head.name) != bannedKeys.end())//if we already have this key we dont handle this object
			return;
		if(!contex.containsKey(head.name))
			return;

		loadProperty(head.value);//continue
	}

	//int/string/double/bool properties; we not nameValuePair so we are inside an array
	template<class T, typename std::enable_if<std::is_arithmetic<typename std::remove_reference<T>::type>::value || std::is_same<typename std::remove_reference<T>::type, String>::value   , sfinae>::type = {}>
	void loadProperty(T& t) {
		auto& contex = contexList.back();
		t = contex.getKeyValue();
		//adding this property to the current contex
		if(!contex.add(t))
			std::cout << "ERROR: failed to set a key in enable_if<std::is_arithmetic" << std::endl;
	}


};

//------------------------------------------- MappingFiles
class DoNothingMappingFile {
public:
	template<typename ModelType, typename JsonObjType, typename ArchiveType>
	void Model2Json(const ModelType& ,const JsonObjType&, const ArchiveType&) {}

	template<typename ... Args>
	void Json2Model(Args& ...) {}
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
		archive.addProperties(DAL::make_nvp("a", a)/*, DAL::make_nvp("sa", sa), DAL::make_nvp("arr", arr)*/);
	}

	template<class Archive>
	void load(Archive& archive){
		archive.loadProperties(DAL::make_nvp("a", a));
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
		archive.addProperties(DAL::make_nvp("ca", ca), DAL::make_nvp("b", b), DAL::make_nvp("brr", brr), DAL::make_nvp("acrr", acrr));
	}
};

void run(){
	B b;
	A a;


	DAL::SerializationService2<DAL::DoNothingMappingFile> *serSevice = new DAL::SerializationService2<DAL::DoNothingMappingFile>(DAL::DoNothingMappingFile());
	std::cout << "the json string is: " << std::endl;
	String generatedJson = serSevice->Model2Json(a);
	std::cout << generatedJson << std::endl;
	serSevice->Json2Model(a, "{\"a\":1111}");
	std::cout << a.a << std::endl;
}

//------------------------------------------------


} /* namespace DAL */

#endif /* SERIALIZATIONSERVICE2_H_ */

// PUT THIS IN SOME OTHER MAIN FILE AND RUN (TO TEST)
/*

#include <memory>
#include <iostream>
#include <stdio.h>
#include <serializationService/JsonSerializationService2.h>
#include <Sprinkler.h>
#include <Plant.h>
#include <Garden.h>
#include <TimePattern.h>
#include <SimpleProgram.h>
using namespace std;


class FlashMappingFile {//mapping file of what we are saving to the flash
public:

template<typename ModelType, typename JsonObjType, typename ArchiveType> inline
void Model2Json(const ModelType& ,const JsonObjType&, const ArchiveType&) {} // all the ones i don't care about go here

template<typename ArchiveType>
void Model2Json(const GardenModel::Plant& plant, DAL::JsonContex& context, ArchiveType&) {//we want the program and sprinkler keys to be the id instead of the whole object
	String key;

	key = "program";
	if(plant._program() != nullptr){// if the plant have the program, adding a key with its id
		context.nextName = &key;
		const int& pid = plant._program()->id();
		context.add(pid);
	}else
		context.listOfNotAllowedKeys.push_back(key);//there wont be a key named program

	key = "sprinkler";
	if(plant._sprinkler() != nullptr){//if sprinkler exists
		context.nextName = &key;
		const int& sid = plant._sprinkler()->id();
		context.add(sid);
	}else
		context.listOfNotAllowedKeys.push_back(key);//we dont want a program key of null, no program = no key in the json file
}
};

class APIMappingFile {//mapping file of what we are saving to the flash
class Link{
public:
	const String rel;
	const String href;
	Link(const String& rel, const String& href): rel(rel), href(href){}

	template <class Archive>
	void save(Archive& archive) const{
		archive.addProperties(MACRO_NVP(rel), MACRO_NVP(href));
	}
};
public:

template<typename ModelType, typename JsonObjType, typename ArchiveType> inline
void Model2Json(const ModelType& ,const JsonObjType&, const ArchiveType&) {} // all the ones i don't care about go here

template<typename ArchiveType>
void Model2Json(const GardenModel::Plant& plant, DAL::JsonContex& context, ArchiveType& archive) {//we want the program and sprinkler keys to be the id instead of the whole object
	String key;
	std::vector<Link> links;

	key = "program";
	if(plant._program() != nullptr){// if the plant have the program, adding a key with its id
		context.nextName = &key;
		const int& pid = plant._program()->id();
		links.push_back(Link(key, "/programs/" + std::to_string(pid) ));//### in the ESP we are using 'String' so we need a different function
	}
	context.listOfNotAllowedKeys.push_back(key);//there wont be a key named program

	key = "sprinkler";
	if(plant._sprinkler() != nullptr){//if sprinkler exists
		context.nextName = &key;
		const int& sid = plant._sprinkler()->id();
		links.push_back(Link(key, "/sprinklers/" + std::to_string(sid)));
	}
	context.listOfNotAllowedKeys.push_back(key);//we dont want a program key of null, no program = no key in the json file

	if(!links.empty())
		archive.addProperties(MACRO_NVP(links));
}
};


int main() {
std::shared_ptr<GardenModel::Sprinkler> sprinkler = std::make_shared<GardenModel::Sprinkler>();;
std::vector<GardenModel::Hour> hours = {GardenModel::Hour(14, 12)};
std::vector<GardenModel::Day> days = {GardenModel::Day(hours)};
GardenModel::TimePattern TPattern(days);
std::shared_ptr<GardenModel::SimpleProgram> simpleProgram = std::make_shared<GardenModel::SimpleProgram>();
simpleProgram->timePattern = TPattern;
std::shared_ptr<GardenModel::Plant> plant = std::make_shared<GardenModel::Plant>(sprinkler, simpleProgram);

GardenModel::Garden garden;
garden._plants.push_back(plant);
garden._sprinklers.push_back(sprinkler);
garden._programs.push_back(simpleProgram);



DAL::SerializationService2<APIMappingFile> *serSevice = new DAL::SerializationService2<APIMappingFile>(APIMappingFile());
std::cout << "the json string is: " << std::endl;
String generatedJson = serSevice->Model2Json(garden);
std::cout << generatedJson << std::endl;

//printf("%d\n", f10);
//DAL::run();
return 0;
}
*/
