/*
 * DefaultSerializationService.h
 *
 *  Created on: 3 баев„ 2018
 *      Author: IsM
 */

#ifndef DALMODULE_SERIALIZATIONSERVICE_DEFAULTSERIALIZATIONSERVICE_H_
#define DALMODULE_SERIALIZATIONSERVICE_DEFAULTSERIALIZATIONSERVICE_H_

#include <DALFramework/serializationService/JsonSerializationService2.h>

#include <DALFramework/serialization/DoNothingMappingFile.h>

namespace DALModule {

class DefaultSerializationService {
	std::shared_ptr<DAL::SerializationService2> serializationService = nullptr;
	mycereal::DoNothingMappingFile mappingFile;
public:
	DefaultSerializationService(std::shared_ptr<DAL::SerializationService2> serializationService): serializationService(serializationService) {}
	~DefaultSerializationService() {}

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

#endif /* DALMODULE_SERIALIZATIONSERVICE_DEFAULTSERIALIZATIONSERVICE_H_ */
