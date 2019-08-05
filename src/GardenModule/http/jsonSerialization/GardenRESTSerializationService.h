/*
 * GardenRESTSerializationService.h
 *
 *  Created on: 3 баев„ 2018
 *      Author: IsM
 */

#ifndef GARDENMODULE_DAL_GARDENRESTSERIALIZATIONSERVICE_H_
#define GARDENMODULE_DAL_GARDENRESTSERIALIZATIONSERVICE_H_

#include <DALFramework/serializationService/JsonSerializationService2.h>

#include <GardenModule/http/jsonSerialization/GardenRESTMappingFile.h>

namespace garden {

class GardenRESTSerializationService {
	std::shared_ptr<DAL::SerializationService2> serializationService = nullptr;
	GardenRESTMappingFile mappingFile;
public:
	GardenRESTSerializationService(std::shared_ptr<DAL::SerializationService2> serializationService) : serializationService(serializationService) {
	//	Serial.println("------------ GardenRESTSerializationService CTOR");
	//	if(serializationService == nullptr)
	//		Serial.println("_______ERROR: In GardenRESTSerializationService CTOR serializationService == nullptr");
	}
	~GardenRESTSerializationService() {
	//	Serial.println("~~~~~~~~~~~~ GardenRESTSerializationService DTOR");
	}

	template<typename ModelType, typename instream>
	void Model2Json(const ModelType& model, instream& stream){//const reference can take rvalues
		serializationService->Model2Json(model, stream, mappingFile);
	}

	//loading only the keys that are in the json if no key is present we just ignore the property, even if it has nameValuePair
	template<typename ModelType, typename outstream>
	void Json2Model(ModelType& model,  outstream& json){//we are taking references and fill them
		serializationService->Json2Model(model, json, mappingFile);
	}

	template<typename ModelType, typename instream>
	void serialize(const ModelType& model, instream& stream){
		Model2Json(model, stream);
	}

};

} /* namespace garden */

#endif /* GARDENMODULE_DAL_GARDENRESTSERIALIZATIONSERVICE_H_ */
