/*
 * StationRESTSerializationService.h
 *
 *  Created on: 4 баев„ 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_REST_STATIONRESTSERIALIZATIONSERVICE_H_
#define STATIONMODULE_REST_STATIONRESTSERIALIZATIONSERVICE_H_

#include <memory>

#include <DALFramework/serializationService/JsonSerializationService2.h>

#include <StationModule/REST/StationRESTMappingFile.h>

namespace sm {

class StationRESTSerializationService {
	std::shared_ptr<DAL::SerializationService2> serializationService = nullptr;
	StationRESTMappingFile mappingFile;
public:
	StationRESTSerializationService(std::shared_ptr<DAL::SerializationService2> serializationService, std::shared_ptr<StationContex> stationContex) : serializationService(serializationService), mappingFile(stationContex){}
	~StationRESTSerializationService() {}

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

} /* namespace sm */

#endif /* STATIONMODULE_REST_STATIONRESTSERIALIZATIONSERVICE_H_ */
