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

#include <DALModule/repositoryPattern/GardenUnitOfWork.h>
#include <DALModule/repositoryPattern/GardenModelContext.h>

#include <DALModule/serialization/json2.h>
#include <DALModule/serializationService/JsonSerializationService.h>


namespace DALModule {

std::shared_ptr<DAL::GardenUnitOfWork> GardenUnitOfWorkCreator(std::shared_ptr<DAL::JsonSerializationService> jsonSerializationService){
	return std::make_shared<DAL::GardenUnitOfWork>(std::make_shared<DAL::GardenModelContext>(jsonSerializationService));
}


std::shared_ptr<DAL::JsonSerializationService> JsonSerializationServiceCreator( ){
	//std::shared_ptr<rapidjson::StringBuffer> jsonSBuffer = std::make_shared<rapidjson::StringBuffer>();
	//std::shared_ptr<rapidjson::Writer<rapidjson::StringBuffer>> writer = std::make_shared<rapidjson::Writer<rapidjson::StringBuffer>>(jsonSBuffer);
	std::shared_ptr<cereal2::JSONOutputArchive> outputArchive =std::make_shared<cereal2::JSONOutputArchive>();

	std::shared_ptr<cereal2::JSONInputArchive> inputArchive = std::make_shared<cereal2::JSONInputArchive>();

	return std::make_shared<DAL::JsonSerializationService>(inputArchive, outputArchive);
}

class DALModule : public MF::ModuleBase  {
public:
	DALModule(){
#ifdef DEBUG_MY_CODE
		Serial.println("DALModule CTOR");
#endif
		 }
	virtual ~DALModule(){
#ifdef DEBUG_MY_CODE
		Serial.println("DALModule DTOR");
#endif
	}

	void start(std::shared_ptr<cntnr::Container> container){
#ifdef DEBUG_MY_CODE
		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
		Serial.println("DALModule start");
#endif
		container->registerType<DAL::GardenUnitOfWork>(&GardenUnitOfWorkCreator, true);//always gives the same unit of work (meaning all the program share the same context model)
		container->registerType<DAL::JsonSerializationService>(&JsonSerializationServiceCreator,true);
	}
};

} /* namespace moduleFramework */

#endif /* DALMODULE_DALMODULE_H_ */
