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
#include <GardenModule/model/GardenVisitor.h>
#include <wString.h>
#include <GardenModule/model/Garden.h>
#include <GardenModule/model/Plant.h>
#include <WString.h>
//#include <stdio.h>

#include <memory>

#include <GardenModule/http/jsonSerialization/GardenRESTSerializationService.h>

namespace garden {

class JsonGardenVisitor: public GardenVisitor {//XXX: TODO: ###you need to remember to free the pointers to the strings you allocate!!!
	std::shared_ptr<garden::GardenRESTSerializationService> gardenRESTSerializationService;
public:
	JsonGardenVisitor(std::shared_ptr<garden::GardenRESTSerializationService> gardenRESTSerializationService) : gardenRESTSerializationService(gardenRESTSerializationService){
	}

	virtual ~JsonGardenVisitor(){
	}

	virtual std::shared_ptr<void> visit(Garden& garden) {
		std::shared_ptr<String> json = std::make_shared<String>();
		if(!json->reserve(2500))
			Serial.println(F("___ERROR: failed to reserve 2500 bytes for the garden json string"));
		gardenRESTSerializationService->Model2Json(garden, *json);
		return json;
	}

	virtual std::shared_ptr<void> visit(Plant& plant) {
		std::shared_ptr<String> json = std::make_shared<String>();
		gardenRESTSerializationService->Model2Json(plant, *json);
		return json;
		//return std::make_shared<String>(_jsonSerializationService->Model2Json(plant));
	}

	virtual std::shared_ptr<void> visit(Sprinkler& sprinkler) {
		std::shared_ptr<String> json = std::make_shared<String>();
		gardenRESTSerializationService->Model2Json(sprinkler, *json);
		return json;
		//std::shared_ptr<void> sptr((char*)json,[=](void*){Serial.println("--------------My costume deleter");});
	}

	virtual std::shared_ptr<void> visit(SimpleProgram& program) {
		std::shared_ptr<String> json = std::make_shared<String>();
		gardenRESTSerializationService->Model2Json(program, *json);
		return json;
	}

};

} /* namespace GardenModel */

#endif /* MODEL_JSONGARDENVISITOR_H_ */
