/*
 * IAPConfAcceptable.h
 *
 *  Created on: 28 αιεμι 2018
 *      Author: IsM
 */

#ifndef ACCESSPOINTMODULE_MODEL_IAPCONFACCEPTABLE_H_
#define ACCESSPOINTMODULE_MODEL_IAPCONFACCEPTABLE_H_

#include <JsonSerializationService2.h>
#include <httpModule/interceptors/SerializationVisitor.h>
#include <DALModule/serialization/DoNothingMappingFile.h>
#include <DALModule/serialization/JsonSaveArchive.h>
#include <DALModule/serialization/JsonLoadArchive.h>

namespace apm {

class IAPConfAcceptable {
public:
	IAPConfAcceptable() {}
	virtual ~IAPConfAcceptable() {}

	typedef DAL::SerializationService2< mycereal::JsonSaveArchive<mycereal::DoNothingMappingFile>,mycereal::JsonLoadArchive<mycereal::DoNothingMappingFile>> serializationServiceType;
	virtual std::shared_ptr<void> accept(Http::SerializationVisitor<serializationServiceType>& visitor) = 0;
};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_MODEL_IAPCONFACCEPTABLE_H_ */
