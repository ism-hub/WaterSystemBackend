/*
 * AccessPointHandlerExecutionChain.h
 *
 *  Created on: 29 ����� 2018
 *      Author: IsM
 */

#ifndef ACCESSPOINTMODULE_RESTAPI_ACCESSPOINTSTATUSHANDLEREXECUTIONCHAIN_H_
#define ACCESSPOINTMODULE_RESTAPI_ACCESSPOINTSTATUSHANDLEREXECUTIONCHAIN_H_

#include <AccessPointModule/status/restAPI/APStatusController.h>
#include <AccessPointModule/status/model/IAPStatusAcceptable.h>
#include <AccessPointModule/status/model/APStatus.h>
#include <HttpFramework.hpp>

#include <DALFramework/serializationService/DefaultSerializationService.h>

namespace apm {

class AccessPointStatusHandlerExecutionChain : public Http::HandlerExecutionChain2<IAPStatusAcceptable, String> {
public:
	AccessPointStatusHandlerExecutionChain(std::shared_ptr<APService> apService, std::shared_ptr<DALModule::DefaultSerializationService> serializationService) :
				Http::HandlerExecutionChain2<IAPStatusAcceptable, String>(std::make_shared<APStatusController>(apService)){
		typedef Http::SerializationVisitor<DALModule::DefaultSerializationService, String> jsonVisitorType;
		typedef Http::SerializationInterceptor<jsonVisitorType, IAPStatusAcceptable, String> jsonInteceptorType;
		auto jsonVisitor = std::make_shared<jsonVisitorType>(serializationService);
		auto jsonInterceptor = std::make_shared<jsonInteceptorType>(jsonVisitor);
		addInterceptor(jsonInterceptor);
		//if(serializationService == nullptr)
		//	Serial.println("__ERRRRRRR__ In AccessPointStatusHandlerExecutionChain - serializationService is null");
	}
	virtual ~AccessPointStatusHandlerExecutionChain() {}
};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_RESTAPI_ACCESSPOINTSTATUSHANDLEREXECUTIONCHAIN_H_ */
