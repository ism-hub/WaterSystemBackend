/*
 * DALModule.h
 *
 * Here we register the unit of work interface (service) (need to add all the repositories and the unit_of_work interfaces and registers them )
 */

#ifndef DALMODULE_DALMODULE_H_
#define DALMODULE_DALMODULE_H_

#include <memory>
#include <ModuleFramework/Module.h>
#include <ModuleFramework/Container/Container.h>

#include <GardenModule/DAL/repositoryPattern/GardenUnitOfWork.h>
#include <GardenModule/DAL/repositoryPattern/GardenModelContext.h>

#include <DALFramework/serialization/DoNothingMappingFile.h>
#include <DALFramework/serializationService/DefaultSerializationService.h>

#include <DALFramework/serializationService/JsonSerializationService2.h>

#include <GardenModule/http/jsonSerialization/GardenRESTSerializationService.h>
#include <GardenModule/DAL/GardenFlashSerializationService.h>


namespace DALModule {

std::shared_ptr<garden::GardenUnitOfWork> GardenUnitOfWorkCreator(std::shared_ptr<garden::GardenFlashSerializationService> gardenFlashSerializationService){
	return std::make_shared<garden::GardenUnitOfWork>(std::make_shared<garden::GardenModelContext>(gardenFlashSerializationService));
}

std::shared_ptr<garden::GardenFlashSerializationService> gardenFlashSerializationServiceCreator(std::shared_ptr<DAL::SerializationService2> SerializationService2){
	return std::make_shared<garden::GardenFlashSerializationService>(SerializationService2);
}

std::shared_ptr<garden::GardenRESTSerializationService> gardenRESTSerializationServiceCreator(std::shared_ptr<DAL::SerializationService2> SerializationService2){
	return std::make_shared<garden::GardenRESTSerializationService>(SerializationService2);
}

std::shared_ptr<DefaultSerializationService> defaultSerializationServiceCreator(std::shared_ptr<DAL::SerializationService2> SerializationService2){
	return std::make_shared<DefaultSerializationService>(SerializationService2);
}

std::shared_ptr<DAL::SerializationService2> SerializationService2Creator(){
	return std::make_shared<DAL::SerializationService2>();
}


class DALModule : public MF::ModuleBase  {
public:
	DALModule(){
	}
	virtual ~DALModule(){
	}

	void start(std::shared_ptr<cntnr::Container> container){
	//	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	//	Serial.println("DALModule start");

		container->registerType<garden::GardenUnitOfWork>(&GardenUnitOfWorkCreator, true);//always gives the same unit of work (meaning all the program share the same context model)
		container->registerType<DAL::SerializationService2>(&SerializationService2Creator, true);
		container->registerType<DefaultSerializationService>(&defaultSerializationServiceCreator, true);
		container->registerType<garden::GardenRESTSerializationService>(&gardenRESTSerializationServiceCreator, true);
		container->registerType<garden::GardenFlashSerializationService>(&gardenFlashSerializationServiceCreator, true);
	//	Serial.println("end Inside DALModule start ");
	}
};

} /* namespace moduleFramework */

#endif /* DALMODULE_DALMODULE_H_ */
