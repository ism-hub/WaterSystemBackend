/*
 * IAPConfAcceptable.h
 *
 *  Created on: 28 ����� 2018
 *      Author: IsM
 */

#ifndef ACCESSPOINTMODULE_MODEL_IAPCONFACCEPTABLE_H_
#define ACCESSPOINTMODULE_MODEL_IAPCONFACCEPTABLE_H_

#include <DALFramework/serializationService/JsonSerializationService2.h>
#include <HttpFramework.hpp>
#include <DALFramework/serializationService/DefaultSerializationService.h>
#include <DALFramework/serialization/JsonSaveArchive.h>
#include <DALFramework/serialization/JsonLoadArchive.h>

namespace apm {

class IAPConfAcceptable {
public:
	IAPConfAcceptable() {}
	virtual ~IAPConfAcceptable() {}

	virtual std::shared_ptr<void> accept(Http::SerializationVisitor<DALModule::DefaultSerializationService, String>& visitor) = 0;
};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_MODEL_IAPCONFACCEPTABLE_H_ */
