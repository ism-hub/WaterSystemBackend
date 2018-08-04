/*
 * AccessPointModule.h
 *
 *
 */

#ifndef ACCESSPOINTMODULE_ACCESSPOINTMODULE_H_
#define ACCESSPOINTMODULE_ACCESSPOINTMODULE_H_

#include <memory>
#include <ModuleFramework/Module.h>
#include <ModuleFramework/Container/Container.h>

#include <ServiceFramework/ServiceFrameWork.h>
#include <SchedulerModule/service/SchedulerService.h>

#include <AccessPointModule/configuration/DAL/APConfContex.h>
#include <AccessPointModule/service/APService.h>

#include <AccessPointModule/configuration/restAPI/AccessPointHandlerExecutionChain.h>
#include <AccessPointModule/status/restAPI/AccessPointStatusHandlerExecutionChain.h>

namespace apm {


std::shared_ptr<APConfContex> APConfContexCreator(std::shared_ptr<DALModule::DefaultSerializationService> serializationService){
	//if(serializationService == nullptr)
		//Serial.println("_______________#$$$$$$$$$ serializationService is NULL");

	std::shared_ptr<APConfContex> apConfContex = std::make_shared<APConfContex>(serializationService, "apconfig.json");
	return apConfContex;
}

std::shared_ptr<Http::IHandlerExecutionChain> AccessPointHandlerExecutionChainCreator(std::shared_ptr<APConfContex> contex, std::shared_ptr<DALModule::DefaultSerializationService> serializationService, std::shared_ptr<APService> apService){
	std::shared_ptr<AccessPointHandlerExecutionChain> exceChain = std::make_shared<AccessPointHandlerExecutionChain>(contex, serializationService, apService);
	return exceChain;
}

std::shared_ptr<APService> APServiceCreator(std::shared_ptr<APConfContex> apConfContex, std::shared_ptr<sched::SchedulerService> scheduler){
	auto apIService = std::make_shared<APService>(scheduler, *(apConfContex->get()));
	return apIService;
}

std::shared_ptr<Http::IHandlerExecutionChain> AccessPointStatusHandlerExecutionChainCreator(std::shared_ptr<APService> apService, std::shared_ptr<DALModule::DefaultSerializationService> serializationService){
	std::shared_ptr<AccessPointStatusHandlerExecutionChain> exceChain = std::make_shared<AccessPointStatusHandlerExecutionChain>(apService, serializationService);
	return exceChain;
}

class AccessPointModule : public MF::ModuleBase  {
public:
	AccessPointModule(){}
	~AccessPointModule(){}

	void start(std::shared_ptr<cntnr::Container> container){
		//Serial.println("Inside AccessPointModule start function ###################");

		container->registerType<APConfContex>(&APConfContexCreator);
		container->registerType<APService>(&APServiceCreator);

		container->registerType<Http::IHandlerExecutionChain>(&AccessPointHandlerExecutionChainCreator);
		container->registerType<Http::IHandlerExecutionChain>(&AccessPointStatusHandlerExecutionChainCreator);


		//starting the AP service
		std::shared_ptr<APService> apService = container->resolve<APService>();

		std::shared_ptr<sfwk::ServiceFrameWork> serviceFramework = container->resolve<sfwk::ServiceFrameWork>();
		serviceFramework->AddService(apService);
		apService->StartService();
		//Serial.println("end Inside AccessPointModule start ");
	}
};

} /* namespace moduleFramework */

#endif /* ACCESSPOINTMODULE_ACCESSPOINTMODULE_H_ */
