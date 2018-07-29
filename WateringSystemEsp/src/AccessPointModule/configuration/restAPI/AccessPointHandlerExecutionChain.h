/*
 * AccessPointHandlerExecutionChain.h
 *
 *  Created on: 29 ����� 2018
 *      Author: IsM
 */

#ifndef ACCESSPOINTMODULE_RESTAPI_ACCESSPOINTHANDLEREXECUTIONCHAIN_H_
#define ACCESSPOINTMODULE_RESTAPI_ACCESSPOINTHANDLEREXECUTIONCHAIN_H_

#include <AccessPointModule/configuration/model/IAPConfAcceptable.h>
#include <AccessPointModule/configuration/restAPI/APConfController.h>
#include <httpModule/dispatcher/HandlerExecutionChain2.h>
#include <httpModule/interceptors/visitorHandlerInterceptor.h>

namespace apm {

class AccessPointHandlerExecutionChain : public Http::HandlerExecutionChain2<IAPConfAcceptable> {
public:
	AccessPointHandlerExecutionChain(std::shared_ptr<APConfContex> contex, std::shared_ptr<apm::SerializationSerice> serializationService, std::shared_ptr<APService> apService) :
				Http::HandlerExecutionChain2<IAPConfAcceptable>(std::make_shared<APConfController>(contex, serializationService, apService)){
		typedef Http::SerializationVisitor<apm::SerializationSerice> jsonVisitorType;
		typedef Http::visitorHandlerInterceptor<jsonVisitorType, IAPConfAcceptable> jsonInteceptorType;
		auto jsonVisitor = std::make_shared<jsonVisitorType>(serializationService);
		auto jsonInterceptor = std::make_shared<jsonInteceptorType>(jsonVisitor);
		addInterceptor(jsonInterceptor);
		if(serializationService == nullptr)
			Serial.println("__ERRRRRRR__ In AccessPointHandlerExecutionChain - serializationService is null");
	}
	virtual ~AccessPointHandlerExecutionChain() {}
};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_RESTAPI_ACCESSPOINTHANDLEREXECUTIONCHAIN_H_ */