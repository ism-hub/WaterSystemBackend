/*
 * JsonLoadArchive.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#ifndef DALMODULE_SERIALIZATION_JSONLOADARCHIVE_H_
#define DALMODULE_SERIALIZATION_JSONLOADARCHIVE_H_

#include <helpers.h>
#include <JsonContex.h>
#include <algorithm>
//#include <iostream>
#include <memory>
#include <ObserverDesignPattern/Property.hpp>
#include <NameValuePair.h>
#include <functional>


namespace mycereal {

template<typename MappingFileType>
class JsonLoadArchive {
	MappingFileType _mappingFile;
	std::vector<DAL::JsonContex> contexList;
	DynamicJsonBuffer jsonBuffer;
	std::vector<std::function<void()> > callbacks;//at the end of the serialization we call them
public:
	JsonLoadArchive(MappingFileType mappingFile):_mappingFile(mappingFile){}
	virtual ~JsonLoadArchive() {}

	//loading only the keys that are in the json if no key is present we just ignore the property, even if it has nameValuePair
	template<typename ModelType>
	void Json2Model(ModelType& model, const String& json){//we are taking references and fill them
		JsonObject& root = jsonBuffer.parseObject(json);
		contexList.push_back(&root);

		//starting the serialization of model by serializing his properties
		_mappingFile.Json2Model(model, *this);//checking to see if the user want to override our default implementation (he can add keys to the banned keys list of the contex and we will ignore them when loading)

		//calling to start to serialize the model properties (adding the keys our user didn't add)
		model.load(*this);

		for(auto& fnc : callbacks)
			fnc();
	}

	void clear(){
		contexList.clear();
		jsonBuffer.clear();
		callbacks.clear();
	}

	void addCallBack(std::function<void()> fnc){
		callbacks.push_back(fnc);
	}

	DAL::JsonContex& getCurrentContex(){
		return contexList.back();
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

		//contex.printAllKeys();

		const std::vector<String>& bannedKeys = contex.listOfNotAllowedKeys;
		if(std::find(bannedKeys.begin(), bannedKeys.end(), head.name) != bannedKeys.end())//if we already have this key we dont handle this object
			return;
		if(head.name == "links")
			int i =2;
		if(!contex.containsKey(head.name))
			return;

		loadProperty(head.value);//continue
	}

protected:

	//int/string/double/bool properties; we not nameValuePair so we are inside an array
	template<class T, typename std::enable_if<std::is_arithmetic<typename std::remove_reference<T>::type>::value || std::is_same<typename std::remove_reference<T>::type, String>::value   , sfinae>::type = {}>
	void loadProperty(T& t) {
		auto& contex = contexList.back();
		t = contex.getKeyValue().operator T();
	}


	//! property with save method - the property is an object, we didn't get NameValuePair so we are inside array
	template<class T, typename std::enable_if<has_load_method<T>::value ,  sfinae>::type = {}>
	void loadProperty(T& t) {
		//the contex in which t is defined
		auto& contex = contexList.back();


		JsonObject& tJson = contex.getKeyValue().operator JsonObject&();
		contexList.push_back(&tJson);//getting t contex (t in json format)
		_mappingFile.Json2Model(t, *this);//if we want to manually handle part of the deserialization (twik the default behaviour hook)
		t.load(*this);//basically calling loadProperties function with the nameValuePairs of the properties we wants from the json and where to put them (refernce)
		contexList.pop_back();//must to remember to pot the context after we done with it (after we done loading that obj)
	}

	//NameValuePair - means that we are in objJson contex, vector mean that the property is an array
	template<class T, typename A>
	void loadProperty(std::vector<T, A>&  vec) {

		//the context where the vector is a property
		auto& contex = contexList.back();

		//the contx which is the Json of the vector
		JsonArray& jArr = contex.getKeyValue().operator JsonArray&();
		contexList.push_back(&jArr);

		//comperator - can check if an json entry of the array have a corresponding element in the list (if the json have instace of it in the list and  we need only to update that instance)
		std::function<T* (std::vector<T, A>&, DAL::JsonContex&)> comperator;
		comperator = _mappingFile.getVectorComperator(vec, contexList.back());//this func can also change vec (dlete/add items)

		//add what is the json, (not touching what is already inside) we can't only update those who are there or delete those who aren't etc.. cause we don't know the keys of the items, the mappingFiles prob deal with it, maybe after i will do the i will return to this TODO: revisit after the Mapping files for the load section is over
		for(int i = 0; i < jArr.size(); i++){
			contexList.back().nextIndx = i;
			T* pt = NULL;
			if(comperator != nullptr){ //not null -> contexList.back().getKeyValue() is a JsonObject
				JsonObject& tJson = contexList.back().getKeyValue().operator JsonObject&();
				DAL::JsonContex jContex = &tJson;
				pt = comperator(vec, jContex);
			}
			if(pt != NULL){//TODO: add something in the lines of contexList.back()[i]
				loadProperty(*pt);
			}else{
				vec.push_back(T());
				loadProperty(vec.back());
			}
		}
		contexList.pop_back();
	}

	//handle property type (invisible)
	template <typename T>
	void loadProperty(Model::Property<T>& prop) {
		T t(prop.get());//copy ctor of that obj. maybe change the whole property notification concept
		loadProperty(t);
		prop = t;
	}

	//handle shared pointer type (invisible)
	template <typename T>
	void loadProperty(std::shared_ptr<T>& sharedPtr) {
		auto& contex = contexList.back();

		if(contex.isNextKeyIsNull()){//maybe we pointing to some value, because we 'merge load' we need to update that to null
			sharedPtr = nullptr;
			return;
		}
		if(sharedPtr == nullptr)//we assume it's ok to create T (call its CTOR) if the user didnt create one
			sharedPtr = std::make_shared<T>();
		loadProperty(*sharedPtr);
	}
};

} /* namespace mycereal */

#endif /* DALMODULE_SERIALIZATION_JSONLOADARCHIVE_H_ */
