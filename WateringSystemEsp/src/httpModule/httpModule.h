/*
 * httpModule.h
 *
 *  Created on: 23 בספט׳ 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_HTTPMODULE_H_
#define HTTPMODULE_HTTPMODULE_H_

#include <vector>
#include <memory>
#include <ModuleFramework/Module.h>
#include <ModuleFramework/Container/Container.h>

#include <httpModule/dispatcher/DispatcherServlet.h>
#include <ESP8266WebServer.h>

#include <httpModule/dispatcher/HandlerExecutionChain.h>
#include <httpModule/controllers/PlantController.h>

#include <httpModule/interceptors/JsonHandlerInterceptor.h>


namespace httpModule {

std::shared_ptr<ESP8266WebServer> ESP8266WebServerCreator(std::shared_ptr<Http::DispatcherServlet> dispatcherServlet){
	return std::make_shared<ESP8266WebServer>(dispatcherServlet);
}

std::shared_ptr<Http::DispatcherServlet> dispatcherServletCreator(std::vector<std::shared_ptr<Http::HandlerExecutionChain>> excecutionChains){
	return std::make_shared<Http::DispatcherServlet>(excecutionChains);
}

std::shared_ptr<Http::HandlerExecutionChain> plantExcecutionChainCreator(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork, std::shared_ptr<Http::JsonHandlerInterceptor> jsonInterceptor){
	std::shared_ptr<Http::HandlerExecutionChain> plantExceChain = std::make_shared<Http::HandlerExecutionChain>(std::make_shared<Http::PlantController>(unitOfWork));
	plantExceChain->addInterceptor(jsonInterceptor);
	return plantExceChain;
}

std::shared_ptr<Http::JsonHandlerInterceptor> jsonHandlerInterceptorCreator(std::shared_ptr<GardenModel::JsonGardenVisitor> jsonGardenVisitor){
	return std::make_shared<Http::JsonHandlerInterceptor>(jsonGardenVisitor);
}

std::shared_ptr<GardenModel::JsonGardenVisitor> jsonGardenVisitorCreator(std::shared_ptr<DAL::JsonSerializationService> jsonSerializationService){
	return std::make_shared<GardenModel::JsonGardenVisitor>(jsonSerializationService);
}


class httpModule : public MF::ModuleBase{
public:
	httpModule() {
		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
		Serial.println("httpModule CTOR"); }
	virtual ~httpModule(){Serial.println("httpModule DTOR");}

	void start(std::shared_ptr<cntnr::Container> container){
		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
		Serial.println("httpModule start");

		container->registerType<Http::DispatcherServlet>(&dispatcherServletCreator, true);
		container->registerType<ESP8266WebServer>(&ESP8266WebServerCreator);

		container->registerType<Http::JsonHandlerInterceptor>(&jsonHandlerInterceptorCreator);
		container->registerType<Http::HandlerExecutionChain>(&plantExcecutionChainCreator);

		container->registerType<GardenModel::JsonGardenVisitor>(&jsonGardenVisitorCreator);
	}
};

} /* namespace DAL */

#endif /* HTTPMODULE_HTTPMODULE_H_ */
