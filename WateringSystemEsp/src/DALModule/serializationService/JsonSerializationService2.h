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
#include <NameValuePair.h>
//#include <Sprinkler.h>

namespace DAL {

//-------------------------------------------- detail input archive


template <typename saveArchiveType, typename loadArchiveType>
class SerializationService2 {
	saveArchiveType& _saveArchive;
	loadArchiveType& _loadArchive;
public:

	SerializationService2(saveArchiveType& saveArchive, loadArchiveType& loadArchive) : _saveArchive(saveArchive), _loadArchive(loadArchive) {}
	virtual ~SerializationService2(){}

	template<typename ModelType, typename instream>
	void Model2Json(const ModelType& model, instream& stream){//const reference can take rvalues
		JsonObject& root = _saveArchive.Model2Json(model);
		root.printTo(stream);
		_saveArchive.clear();
	}

	//loading only the keys that are in the json if no key is present we just ignore the property, even if it has nameValuePair
	template<typename ModelType>
	void Json2Model(ModelType& model, const String& json){//we are taking references and fill them
		_loadArchive.Json2Model(model, json);
		_loadArchive.clear();
	}


};

//------------------------------------------- MappingFiles

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
		archive.addProperties(mycereal::make_nvp("a", a), mycereal::make_nvp("sa", sa), mycereal::make_nvp("arr", arr));
	}

	template<class Archive>
	void load(Archive& archive){
		archive.loadProperties(mycereal::make_nvp("a", a), mycereal::make_nvp("sa", sa), mycereal::make_nvp("arr", arr));
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
		archive.addProperties(mycereal::make_nvp("ca", ca), mycereal::make_nvp("b", b), mycereal::make_nvp("brr", brr), mycereal::make_nvp("acrr", acrr));
	}

	template<class Archive>
	void load( Archive& archive) {
		archive.loadProperties(mycereal::make_nvp("ca", ca), mycereal::make_nvp("b", b), mycereal::make_nvp("brr", brr), mycereal::make_nvp("acrr", acrr));
	}
};

void run(){
	/*B b;
	A a;


	DAL::SerializationService2<DAL::DoNothingMappingFile> *serSevice = new DAL::SerializationService2<DAL::DoNothingMappingFile>(DAL::DoNothingMappingFile());
	std::cout << "Before: " << std::endl;
	String generatedJson = serSevice->Model2Json(b);
	std::cout << generatedJson << std::endl;
	serSevice->Json2Model(b, "{\"ca\":{\"a\":123,\"sa\":\"kibataMaka\",\"arr\":[11,22,33]},\"b\":22,\"brr\":[12,23,25],\"acrr\":[{\"a\":10,\"sa\":\"kibata0\",\"arr\":[10,20,30]},{\"a\":19,\"sa\":\"kibata9\",\"arr\":[19,29,39]},{\"a\":17,\"sa\":\"kibata7\",\"arr\":[17,27,37]}]}");
	std::cout << "After: " << serSevice->Model2Json(b) << std::endl;*/
}

//------------------------------------------------


} /* namespace DAL */

#endif /* SERIALIZATIONSERVICE2_H_ */

// PUT THIS IN SOME OTHER MAIN FILE AND RUN (TO TEST)
/*

*/
