/*
 * APStatusAcceptable.h
 *
 *  Created on: 29 ����� 2018
 *      Author: IsM
 */

#ifndef ACCESSPOINTMODULE_STATUS_MODEL_IAPSTATUSACCEPTABLE_H_
#define ACCESSPOINTMODULE_STATUS_MODEL_IAPSTATUSACCEPTABLE_H_


#include <DALModule/DALModule.h>
#include <httpModule/interceptors/SerializationVisitor.h>

namespace apm {

class IAPStatusAcceptable {
public:
	IAPStatusAcceptable() {}
	virtual ~IAPStatusAcceptable() {}

	virtual std::shared_ptr<void> accept(Http::SerializationVisitor<DALModule::DefaultSerializationServerType>& visitor) = 0;
};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_STATUS_MODEL_IAPSTATUSACCEPTABLE_H_ */
