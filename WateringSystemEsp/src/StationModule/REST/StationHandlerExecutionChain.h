/*
 * StationHandlerExecutionChain.h
 *
 *  Created on: 31 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_REST_STATIONHANDLEREXECUTIONCHAIN_H_
#define STATIONMODULE_REST_STATIONHANDLEREXECUTIONCHAIN_H_

#include <memory>

#include <HttpFramework/dispatcher/HandlerExecutionChain2.h>
#include <HttpFramework/inteceptors/visitorHandlerInterceptor.h>
#include <HttpFramework/dispatcher/HandlerExecutionChain2.h>
#include <HttpFramework/inteceptors/visitorHandlerInterceptor.h>

#include <StationModule/model/IStationAcceptable.h>
#include <StationModule/REST/StationController.h>
#include <StationModule/REST/StationRESTJsonVisitor.h>
#include <StationModule/REST/StationRESTSerializationService.h>

namespace sm {

class StationHandlerExecutionChain : public Http::HandlerExecutionChain2<IStationAcceptable> {
public:
	StationHandlerExecutionChain(std::shared_ptr<StationIService> stationService, std::shared_ptr<StationRESTSerializationService> _restSerializationService) :
			Http::HandlerExecutionChain2<IStationAcceptable>(std::make_shared<StationController>(stationService, _restSerializationService)){
	//	Serial.println("------------------------------ StationHandlerExecutionChain CTOR");
		//typedef Http::SerializationVisitor<StationRESTSerializationService> jsonVisitorType;
		typedef Http::visitorHandlerInterceptor<StationRESTJsonVisitor, IStationAcceptable> jsonInteceptorType;
		auto jsonVisitor = std::make_shared<StationRESTJsonVisitor>(_restSerializationService);
		auto jsonInterceptor = std::make_shared<jsonInteceptorType>(jsonVisitor);
		addInterceptor(jsonInterceptor);
	//	if(_restSerializationService == nullptr || stationService == nullptr)
	//		Serial.println("__ERRRRRRR__ In StationHandlerExecutionChain - _restSerializationService is null || stationService == nullptr");
	//	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	//	Serial.println("------------------------------ END StationHandlerExecutionChain CTOR");
	}
	 ~StationHandlerExecutionChain() {
	//	Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~ StationHandlerExecutionChain DTOR");
	}
};

} /* namespace sm */

#endif /* STATIONMODULE_REST_STATIONHANDLEREXECUTIONCHAIN_H_ */
