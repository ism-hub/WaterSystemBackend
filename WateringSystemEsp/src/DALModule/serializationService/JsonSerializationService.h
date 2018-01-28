/*
 * SerializationService.h
 *
 *  Created on: 22 בספט׳ 2017
 *      Author: IsM
 */

#ifndef DALMODULE_JSONSERIALIZATIONSERVICE_JSONSERIALIZATIONSERVICE_H_
#define DALMODULE_JSONSERIALIZATIONSERVICE_JSONSERIALIZATIONSERVICE_H_

#include <WString.h>
#include <Model/GardenAcceptable.h>
#include <memory>

#include <DALModule/serialization/json2.h>
//#include <Model/visitors/JsonGardenVisitor.h>

#include <ModuleFramework/utils.h>

namespace DAL {

class JsonSerializationService {
	//std::shared_ptr<GardenModel::JsonGardenVisitor>	jsonGardenVisitor;
	std::shared_ptr<cereal2::JSONInputArchive> 		_inputArchive;
	std::shared_ptr<cereal2::JSONOutputArchive> 	_outputArchive;
	//I'm letting access to it in order to minimize the number of allocations when dealing with strings
	//the main use is to past the json test into it for jsonToModel
	rapidjson::StringBuffer							_stringBuffer;
public:



	JsonSerializationService(std::shared_ptr<cereal2::JSONInputArchive> inputArchive, std::shared_ptr<cereal2::JSONOutputArchive> outputArchive):
		_inputArchive(inputArchive),
		_outputArchive(outputArchive)
	{

	}

	virtual ~JsonSerializationService(){

	}

	template <class T>
	String modelToJson(T& model){
		//std::shared_ptr<rapidjson::StringBuffer> jsonSBuffer = std::make_shared<rapidjson::StringBuffer>();
		//std::shared_ptr<rapidjson::Writer<rapidjson::StringBuffer>> writer = std::make_shared<rapidjson::Writer<rapidjson::StringBuffer>>(jsonSBuffer);
		_stringBuffer.Clear();
		_outputArchive->reset(_stringBuffer);//_outputArchive->itsWriter.reset(jsonSBuffer);
		_outputArchive->operator ()(cereal2::make_nvp(MF::getTemplateName<T>(), model));
		_outputArchive->finalize();// instead of _outputArchive->~JSONOutputArchive(); for reusing capabilities

#ifdef DEBUG_MY_CODE
		Serial.println("The json String from modelToJson(T& model):");
		Serial.println(_stringBuffer.GetString());
#endif
		return _stringBuffer.GetString();
	}

	//pre: already inserted the string we want to _stringBuffer
	template <class T>
	std::shared_ptr<T> jsonToModel(String json){

#ifdef DEBUG_MY_CODE
		Serial.println("The json String we will jsonToModel:");
		Serial.println(json);
#endif

		_inputArchive->reset(json.c_str());//sets new sink
		std::shared_ptr<T> model = std::make_shared<T>();
		(*_inputArchive)(*model);

		return model;
	}



	rapidjson::StringBuffer& getInternalBuffer()
	{
		return _stringBuffer;
	}

};

} /* namespace DAL */

#endif /* DALMODULE_SERIALIZATIONSERVICE_SERIALIZATIONSERVICE_H_ */
