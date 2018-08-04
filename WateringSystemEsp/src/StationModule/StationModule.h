/*
 * StationModule.h
 *
 *  Created on: 29 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_STATIONMODULE_H_
#define STATIONMODULE_STATIONMODULE_H_

#include <memory>
#include <ModuleFramework/Module.h>
#include <ModuleFramework/Container/Container.h>

#include <ServiceFramework/ServiceFrameWork.h>
#include <SchedulerModule/service/SchedulerService.h>



#include <StationModule/service/StationIService.h>
#include <StationModule/DAL/StationContex.h>
#include <StationModule/DAL/StationFlashMappingFile.h>

#include <StationModule/REST/StationController.h>
#include <StationModule/REST/StationHandlerExecutionChain.h>
#include <StationModule/REST/StationRESTSerializationService.h>

namespace sm {

std::shared_ptr<Http::IHandlerExecutionChain> stationHandlerExecutionChainCreator(std::shared_ptr<StationIService> stationService, std::shared_ptr<StationRESTSerializationService> restSerializationService) {
//	if(stationService == nullptr || restSerializationService == nullptr)
//			Serial.println("________________ERROR________ stationHandlerExecutionChainCreator  stationService == nullptr || restSerializationService == nullptr");
	return std::make_shared<StationHandlerExecutionChain>(stationService, restSerializationService);
}

std::shared_ptr<StationRESTSerializationService> stationRESTSerializationServiceCreator(std::shared_ptr<DAL::SerializationService2> serializationService, std::shared_ptr<StationContex> stationContex){
//	if(stationContex == nullptr)
//		Serial.println("________________ERROR________ stationRESTSerializationServiceCreator  stationContex == null");
	return std::make_shared<StationRESTSerializationService>(serializationService, stationContex);
}

std::shared_ptr<StationFlashSerializationService> stationFlashSerializationServiceCreator(std::shared_ptr<DAL::SerializationService2> serializationService){
	return std::make_shared<StationFlashSerializationService>(serializationService);
}


std::shared_ptr<StationContex> stationContexCreator(std::shared_ptr<StationFlashSerializationService> serializationService, std::shared_ptr<sched::SchedulerService> scheduler){
//	if(serializationService == nullptr || scheduler == nullptr)
//		Serial.println("_______________#$$$$$$$$$ StationFlashSerializationServiceType is NULL || scheduler == nullptr");
	std::shared_ptr<StationContex> stationContex = std::make_shared<StationContex>(serializationService, scheduler, "station.json");
	return stationContex;
}

std::shared_ptr<StationIService> StationIServiceCreator(std::shared_ptr<StationContex> stationContex){
//	if(stationContex == nullptr)
//			Serial.println("________________ERROR________ StationIServiceCreator  stationContex == null");
	auto apIService = std::make_shared<StationIService>(stationContex);
	return apIService;
}

class StationModule : public MF::ModuleBase {
public:
	StationModule() {}
	~StationModule() {}

	void start(std::shared_ptr<cntnr::Container> container) {
	//	Serial.println("Inside StationModule start ");
		container->registerType<StationFlashSerializationService>(&stationFlashSerializationServiceCreator);
		container->registerType<StationContex>(&stationContexCreator);
		container->registerType<StationIService>(&StationIServiceCreator);

		container->registerType<StationRESTSerializationService>(&stationRESTSerializationServiceCreator);
		container->registerType<Http::IHandlerExecutionChain>(&stationHandlerExecutionChainCreator);


		//starting the server
		std::shared_ptr<StationIService> stationService = container->resolve<StationIService>();
		std::shared_ptr<sfwk::ServiceFrameWork> serviceFramework = container->resolve<sfwk::ServiceFrameWork>();
		serviceFramework->AddService(stationService);
		stationService->StartService();
	}
};

} /* namespace sm */

#endif /* STATIONMODULE_STATIONMODULE_H_ */
