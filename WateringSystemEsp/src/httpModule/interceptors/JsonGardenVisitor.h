/*
 * JsonGardenVisitor.h
 *
 *  Created on: 22 ����� 2017
 *      Author: IsM
 */

#ifndef MODEL_JSONGARDENVISITOR_H_
#define MODEL_JSONGARDENVISITOR_H_

#include <DALModule/serialization/cereal2.h>
#include <DALModule/serialization/json2.h>
#include <GardenVisitor.h>
#include <wString.h>
#include <Garden.h>
#include <Plant.h>
#include <WString.h>
#include <stdio.h>



namespace GardenModel {

class JsonGardenVisitor: public GardenVisitor {//###you need to remember to free the pointers to the strings you allocate!!!
public:

	std::shared_ptr<DAL::JsonSerializationService>	_jsonSerializationService;

	JsonGardenVisitor(std::shared_ptr<DAL::JsonSerializationService> jsonSerializationService) : _jsonSerializationService(jsonSerializationService){
#ifdef DEBUG_MY_CODE
		Serial.println("JsonGardenVisitor CTOR");
#endif
	}

	virtual ~JsonGardenVisitor(){
#ifdef DEBUG_MY_CODE
		Serial.println("JsonGardenVisitor DTOR");
#endif
	}

	virtual std::shared_ptr<void> visit(Garden& garden) {
		return std::make_shared<String>(_jsonSerializationService->modelToJson(garden));
	}

	virtual std::shared_ptr<void> visit(Plant& plant) {
		return std::make_shared<String>(_jsonSerializationService->modelToJson(plant));
	}

	virtual std::shared_ptr<void> visit(Sprinkler& sprinkler) {
		return std::make_shared<String>(_jsonSerializationService->modelToJson(sprinkler));
		//std::shared_ptr<void> sptr((char*)json,[=](void*){Serial.println("--------------My costume deleter");});
	}

	virtual std::shared_ptr<void> visit(SimpleProgram& program) {
		return std::make_shared<String>(_jsonSerializationService->modelToJson(program));
	}

};

} /* namespace GardenModel */

#endif /* MODEL_JSONGARDENVISITOR_H_ */
