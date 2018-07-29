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

#include <ServiceFrameWork.h>
#include <SchedulerService.h>
#include <AccessPointModule/DAL/APConfContex.h>

#include <AccessPointModule/service/APService.h>

#include <AccessPointModule/restAPI/AccessPointHandlerExecutionChain.h>

namespace apm {


std::shared_ptr<APConfContex> APConfContexCreator(std::shared_ptr<SerializationSerice> serializationService){
	if(serializationService == nullptr)
		Serial.println("_______________#$$$$$$$$$ serializationService is NULL");
	std::shared_ptr<APConfContex> apConfContex = std::make_shared<APConfContex>(serializationService, "apconfig.json");
	return apConfContex;
}

std::shared_ptr<Http::IHandlerExecutionChain> AccessPointHandlerExecutionChainCreator(std::shared_ptr<APConfContex> contex, std::shared_ptr<DALModule::DefaultSerializationServerType> serializationService){
	std::shared_ptr<AccessPointHandlerExecutionChain> exceChain = std::make_shared<AccessPointHandlerExecutionChain>(contex, serializationService);
	return exceChain;
}


class AccessPointModule : public MF::ModuleBase  {
public:
	AccessPointModule(){}
	virtual ~AccessPointModule(){}

	void start(std::shared_ptr<cntnr::Container> container){
		Serial.println("Inside AccessPointModule start function ###################");

		container->registerType<APConfContex>(&APConfContexCreator);

		container->registerType<Http::IHandlerExecutionChain>(&AccessPointHandlerExecutionChainCreator);

		//creating adding and starting the service
		std::shared_ptr<APConfContex> apConfContex = container->resolve<APConfContex>();
		std::shared_ptr<sched::SchedulerService> scheduleService = container->resolve<sched::SchedulerService>();
		auto apIService = std::make_shared<APService>(scheduleService, *(apConfContex->get()));
		std::shared_ptr<sfwk::ServiceFrameWork> serviceFramework = container->resolve<sfwk::ServiceFrameWork>();
		serviceFramework->AddService(apIService);
		apIService->StartService();
	}
};

} /* namespace moduleFramework */

#endif /* ACCESSPOINTMODULE_ACCESSPOINTMODULE_H_ */
