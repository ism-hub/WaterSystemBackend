/*
 * JsonSaveArchive.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#ifndef DALMODULE_SERIALIZATION_JSONSAVEARCHIVE_H_
#define DALMODULE_SERIALIZATION_JSONSAVEARCHIVE_H_

#include <DALFramework/serialization/helpers.h>
#include <DALFramework/serialization/JsonContex.h>
#include <algorithm>
//#include <iostream>
#include <memory>
#include <ObserverDesignPattern/Property.hpp>
#include <DALFramework/serialization/NameValuePair.h>

namespace mycereal {


class JsonSaveArchive {
	DynamicJsonBuffer& jsonBuffer;
	std::vector<DAL::JsonContex>& contexList;
public:
	//JsonSaveArchive(MappingFileType mappingFile):_mappingFile(mappingFile){
//		Serial.println("------------ JsonSaveArchive CTOR");
//		Serial.println(__PRETTY_FUNCTION__);
	//}
	JsonSaveArchive(DynamicJsonBuffer& jsonBuffer, std::vector<DAL::JsonContex>& contexList) : jsonBuffer(jsonBuffer), contexList(contexList){
	}
	~JsonSaveArchive() {
	}

	//don't forget to call clean after using this method!!
	template<typename ModelType, typename MappingFileType>
	JsonObject& Model2Json(const ModelType& model, MappingFileType& mappingFile){//const reference can take rvalues
		// creating the json contex
		JsonObject& root = jsonBuffer.createObject();
		contexList.push_back(&root);

		//starting the serialization of model by serializing his properties
		mappingFile.Model2Json(model, *this);//checking to see if the user want to override our default implementation (buy mannualy adding some keys)
		//calling to start to serialize the model properties (adding the keys our user didn't add)
		model.save(mappingFile, *this);

		return root;
	}

	void clear(){
		contexList.clear();
		jsonBuffer.clear();
	}

	DAL::JsonContex& getCurrentContex(){
		return contexList.back();
	}

	//! Unwinds to process all data
	template <typename MappingFileType, class T, class ... Other> inline
	void addProperties(MappingFileType& mappingFile, const T& head, const Other& ... tail )
	{
		addProperty(head, mappingFile);
		addProperties(mappingFile, tail ... );
	}
	template <typename MappingFileType>
	void addProperties(MappingFileType& ){}


	//catching all the NameValuePairs. we save its name as the nextName for the json context
	//if we handle a NameValuePair it means that we inside an object and we want to add to it a property named head.name
	//if a property names like that already exists we dont cont (the mapping file already custom added it)
	template<class T, typename MappingFileType>
	void addProperty(const NameValuePair<T>& head, MappingFileType& mappingFile){
		/*Serial.print(F("Inside addProperty(const NameValuePair... the name is:"));Serial.println(head.name);
		Serial.print(F("settings heap size: "));Serial.println(ESP.getFreeHeap());
		Serial.print(F("num of elements in the contexList: "));Serial.println(contexList.size());
		Serial.print(F("capacity of contexList: "));Serial.println(contexList.capacity());
		Serial.print(F("jsonBuffer.size(): "));Serial.println(jsonBuffer.size());*/
		auto& contex = contexList.back();
		contex.nextName = &head.name;//save the name

		const std::vector<String>& bannedKeys = contex.listOfNotAllowedKeys;
		if(contex.containsKey(head.name) || std::find(bannedKeys.begin(), bannedKeys.end(), head.name) != bannedKeys.end())//if we already have this key we dont handle this object
			return;

		addProperty(head.value, mappingFile);//continue
	}
protected:
	//! property with save method - the property is an object,
	template<class T, typename std::enable_if<has_save_method<T>::value ,  sfinae>::type = {}, typename MappingFileType>
	void addProperty(const T& t, MappingFileType& mappingFile) {
		//getting the correct contex
		auto& contex = contexList.back();

		//adding this property to the current contex
		//JsonContex propContex = contex.createNestedObject();//add here a destructor that pops the element from the list by itself?
		contexList.push_back(contex.createNestedObject());
		mappingFile.Model2Json(t, *this);
		t.save(mappingFile, *this);//adding this property properties (basically like calling 'addProperties' with all the properties the user want)
		contexList.pop_back();//must to remember to pot the context after we done with it (do some auto pop context thing on scope destruction..)
	}

	//int/string/double/bool properties;
	template<class T, typename std::enable_if<std::is_arithmetic<typename std::remove_reference<T>::type>::value || std::is_same<typename std::remove_reference<T>::type, String>::value   , sfinae>::type = {}, typename MappingFileType>
	void addProperty(const T& t, MappingFileType& ) {
		auto& contex = contexList.back();

		delay(0);
		//adding this property to the current contex
		if(!contex.add(t)){
			Serial.println(F("ERROR: failed to set a key in enable_if<std::is_arithmetic"));
			//Serial.println(__PRETTY_FUNCTION__);
			Serial.print(F("is the contex which we add the value is Object: "));Serial.println(contex.isJsonObject());

			if(contex.isNextKeyIsNull()){
				Serial.print(F("The next key is null "));
			}else{
				Serial.print(F("The next key is: "));Serial.println(*contex.nextName);
			}
			Serial.print(F("The value: "));Serial.println(t);
			contex.printAllKeys();
		}
			//std::cout << "ERROR: failed to set a key in enable_if<std::is_arithmetic" << std::endl;
	}


	//NameValuePair - means that we are in objJson contex, vector mean that the property is an array
	template<class T, typename A, typename MappingFileType>
	void addProperty(std::vector<T, A> const &  vec, MappingFileType& mappingFile) {

		//getting the correct contex
		auto& contex = contexList.back();

		//creating new array contex
		DAL::JsonContex propJsonArrCntx = contex.createNestedArray();
		contexList.push_back(propJsonArrCntx);

		//adding all the array items
		for(const auto& t : vec){
			addProperty(t, mappingFile);
		}

		contexList.pop_back();
	}

	//handle property type (invisible)
	template <typename T, typename MappingFileType>
	void addProperty(Model::Property<T> const & prop, MappingFileType& mappingFile) {
		addProperty(prop.get(), mappingFile);
	}

	//handle shared pointer type (invisible)
	template <typename T, typename MappingFileType>
	void addProperty(const std::shared_ptr<T>& sharedPtr, MappingFileType& mappingFile) {
		if(sharedPtr != nullptr)
			addProperty(*sharedPtr, mappingFile);
		else{// if we null ptr we need to add null property
			if(!contexList.back().add((char*)0))
				Serial.println(F("ERROR: failed to set null value"));
		}
	}

};

} /* namespace mycereal */

#endif /* DALMODULE_SERIALIZATION_JSONSAVEARCHIVE_H_ */
