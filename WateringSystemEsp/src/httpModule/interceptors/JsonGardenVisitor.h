/*
 * JsonGardenVisitor.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef MODEL_JSONGARDENVISITOR_H_
#define MODEL_JSONGARDENVISITOR_H_

//#include <DALModule/serialization/cereal2.h>
//#include <DALModule/serialization/json2.h>
#include <GardenVisitor.h>
#include <wString.h>
#include <Garden.h>
#include <Plant.h>
#include <WString.h>
#include <stdio.h>
#include <JsonLoadArchive.h>
#include <JsonSaveArchive.h>
//#include <SerializationService2.h>
#include <APIMappingFile.h>
#include <memory>

namespace GardenModel {

class JsonGardenVisitor: public GardenVisitor {//###you need to remember to free the pointers to the strings you allocate!!!
public:

	std::shared_ptr<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::APIMappingFile>, mycereal::JsonLoadArchive<DAL::APIMappingFile>>>	_jsonSerializationService;

	JsonGardenVisitor(std::shared_ptr<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::APIMappingFile>, mycereal::JsonLoadArchive<DAL::APIMappingFile>> > jsonSerializationService) : _jsonSerializationService(jsonSerializationService){
	}

	virtual ~JsonGardenVisitor(){
	}

	virtual std::shared_ptr<void> visit(Garden& garden) {
		std::shared_ptr<String> json = std::make_shared<String>();
		_jsonSerializationService->Model2Json(garden, *json);
		return json;
	}

	virtual std::shared_ptr<void> visit(Plant& plant) {
		std::shared_ptr<String> json = std::make_shared<String>();
		_jsonSerializationService->Model2Json(plant, *json);
		return json;
		//return std::make_shared<String>(_jsonSerializationService->Model2Json(plant));
	}

	virtual std::shared_ptr<void> visit(Sprinkler& sprinkler) {
		std::shared_ptr<String> json = std::make_shared<String>();
		_jsonSerializationService->Model2Json(sprinkler, *json);
		return json;
		//std::shared_ptr<void> sptr((char*)json,[=](void*){Serial.println("--------------My costume deleter");});
	}

	virtual std::shared_ptr<void> visit(SimpleProgram& program) {
		std::shared_ptr<String> json = std::make_shared<String>();
		_jsonSerializationService->Model2Json(program, *json);
		return json;
	}

};

} /* namespace GardenModel */

#endif /* MODEL_JSONGARDENVISITOR_H_ */
