/*
 * StationFlashSerializationService.h
 *
 *  Created on: 4 баев„ 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_DAL_STATIONFLASHSERIALIZATIONSERVICE_H_
#define STATIONMODULE_DAL_STATIONFLASHSERIALIZATIONSERVICE_H_

#include <memory>

#include <DALFramework/serializationService/JsonSerializationService2.h>

#include <StationModule/DAL/StationFlashMappingFile.h>

namespace sm {

class StationFlashSerializationService {
	std::shared_ptr<DAL::SerializationService2> serializationService = nullptr;
	StationFlashMappingFile mappingFile;
public:
	StationFlashSerializationService(std::shared_ptr<DAL::SerializationService2> serializationService) : serializationService(serializationService) {}
	~StationFlashSerializationService(){}

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

} /* namespace DALModule */

#endif /* STATIONMODULE_DAL_STATIONFLASHSERIALIZATIONSERVICE_H_ */
