/*
 * JsonSaveArchive.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#ifndef DALMODULE_SERIALIZATION_JSONSAVEARCHIVE_H_
#define DALMODULE_SERIALIZATION_JSONSAVEARCHIVE_H_

#include <helpers.h>
#include <JsonContex.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <ObserverDesignPattern/Property.hpp>
#include <NameValuePair.h>

namespace mycereal {

template<typename MappingFileType>
class JsonSaveArchive {
	MappingFileType _mappingFile;
	std::vector<DAL::JsonContex> contexList;
	DynamicJsonBuffer jsonBuffer;
public:
	JsonSaveArchive(MappingFileType mappingFile):_mappingFile(mappingFile){}
	virtual ~JsonSaveArchive() {}

	//don't forget to call clean after using this method!!
	template<typename ModelType>
	JsonObject& Model2Json(const ModelType& model){//const reference can take rvalues
		// creating the json contex
		JsonObject& root = jsonBuffer.createObject();
		contexList.push_back(&root);

		//starting the serialization of model by serializing his properties
		_mappingFile.Model2Json(model, *this);//checking to see if the user want to override our default implementation (buy mannualy adding some keys)

		//calling to start to serialize the model properties (adding the keys our user didn't add)
		model.save(*this);

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
	template <class T, class ... Other> inline
	void addProperties(const T& head, const Other& ... tail )
	{
		addProperty(head );
		addProperties( tail ... );
	}
	void addProperties(){}


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
protected:
	//! property with save method - the property is an object, we didn't get NameValuePair so we are inside array
	template<class T, typename std::enable_if<has_save_method<T>::value ,  sfinae>::type = {}>
	void addProperty(const T& t) {
		//getting the correct contex
		auto& contex = contexList.back();

		//adding this property to the current contex
		//JsonContex propContex = contex.createNestedObject();//add here a destructor that pops the element from the list by itself?
		contexList.push_back(contex.createNestedObject());
		_mappingFile.Model2Json(t, *this);
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
		DAL::JsonContex propJsonArrCntx = contex.createNestedArray();
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

};

} /* namespace mycereal */

#endif /* DALMODULE_SERIALIZATION_JSONSAVEARCHIVE_H_ */
