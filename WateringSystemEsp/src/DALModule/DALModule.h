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

//#include <DALModule/serialization/json2.h>
#include <JsonSerializationService2.h>
#include <JsonSaveArchive.h>
#include <JsonLoadArchive.h>
#include <FlashMappingFile.h>
#include <APIMappingFile.h>


namespace DALModule {

std::shared_ptr<DAL::GardenUnitOfWork> GardenUnitOfWorkCreator(std::shared_ptr<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::FlashMappingFile>, mycereal::JsonLoadArchive<DAL::FlashMappingFile>>> jsonFlashSerializationService){
	return std::make_shared<DAL::GardenUnitOfWork>(std::make_shared<DAL::GardenModelContext>(jsonFlashSerializationService));
}

template<typename MappingFileType>
std::shared_ptr<DAL::SerializationService2<mycereal::JsonSaveArchive<MappingFileType>, mycereal::JsonLoadArchive<MappingFileType>> > createSerializationServer(MappingFileType& mappingFile) {
	typedef mycereal::JsonLoadArchive<MappingFileType> LoadArchiveType;
	typedef mycereal::JsonSaveArchive<MappingFileType> SaveArchiveType;
	typedef DAL::SerializationService2<SaveArchiveType, LoadArchiveType > ServerType;

	auto loadArchive = std::make_shared<LoadArchiveType>(mappingFile);
	auto saveArchive = std::make_shared<SaveArchiveType>(mappingFile);
	auto serSevice =  std::make_shared<ServerType>(saveArchive, loadArchive);
	return serSevice;
}

typedef DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::FlashMappingFile>,
													mycereal::JsonLoadArchive<DAL::FlashMappingFile>> FlashSerializationServerType;

std::shared_ptr<FlashSerializationServerType> JsonFlashSerializationServiceCreator( ){
	DAL::FlashMappingFile flashMappingFile;
	return createSerializationServer<DAL::FlashMappingFile>(flashMappingFile);
}

typedef DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::APIMappingFile>,
													mycereal::JsonLoadArchive<DAL::APIMappingFile>> APISerializationServerType;

std::shared_ptr<APISerializationServerType> JsonAPISerializationServiceCreator( ){
	DAL::APIMappingFile flashMappingFile;
	return createSerializationServer<DAL::APIMappingFile>(flashMappingFile);
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
		container->registerType<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::FlashMappingFile>, mycereal::JsonLoadArchive<DAL::FlashMappingFile>> >(&JsonFlashSerializationServiceCreator, true);
		container->registerType<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::APIMappingFile>, mycereal::JsonLoadArchive<DAL::APIMappingFile>> >(&JsonAPISerializationServiceCreator, true);

	}
};

} /* namespace moduleFramework */

#endif /* DALMODULE_DALMODULE_H_ */
