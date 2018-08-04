/*
 * SerializationService2.h
 *
 *  Created on: 8 áôáø× 2018
 *      Author: rina
 */

#ifndef SERIALIZATIONSERVICE2_H_
#define SERIALIZATIONSERVICE2_H_

#include <HardwareSerial.h>

//#include <iostream>
#include <vector>
#include <ArduinoJson.h>
#include <WString.h>
#include <ObserverDesignPattern/Property.hpp>
#include <algorithm>
#include <DALFramework/serialization/NameValuePair.h>
//#include <Sprinkler.h>

#include <Arduino.h>

#include <DALFramework/serialization/JsonLoadArchive.h>
#include <DALFramework/serialization/JsonSaveArchive.h>

namespace DAL {

//-------------------------------------------- detail input archive


class SerializationService2 {
	std::vector<DAL::JsonContex> contexList;
	DynamicJsonBuffer jsonBuffer;
	mycereal::JsonSaveArchive _saveArchive;
	mycereal::JsonLoadArchive _loadArchive;

public:


	SerializationService2() : _saveArchive(jsonBuffer, contexList), _loadArchive(jsonBuffer, contexList){
	}
	~SerializationService2(){
	}

	template<typename ModelType, typename instream, typename MappingFile>
	void Model2Json(const ModelType& model, instream& stream, MappingFile& mappingFile){//const reference can take rvalues
		JsonObject& root = _saveArchive.Model2Json(model, mappingFile);
		delay(0);
		root.printTo(stream);
		_saveArchive.clear();
	}

	//loading only the keys that are in the json if no key is present we just ignore the property, even if it has nameValuePair
	template<typename ModelType, typename outstream, typename MappingFile>
	void Json2Model(ModelType& model,  outstream& json, MappingFile& mappingFile){//we are taking references and fill them
		delay(0);
		_loadArchive.Json2Model(model, json, mappingFile);
		_loadArchive.clear();
	}

	template<typename ModelType, typename instream, typename MappingFile>
	void serialize(const ModelType& model, instream& stream, MappingFile& mappingFile){
		Model2Json(model, stream, mappingFile);
	}

};

} /* namespace DAL */

#endif /* SERIALIZATIONSERVICE2_H_ */

// PUT THIS IN SOME OTHER MAIN FILE AND RUN (TO TEST)
/*

*/
