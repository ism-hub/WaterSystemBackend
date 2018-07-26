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

#include <httpModule/model/HttpServletRequest.h>
#include <httpModule/model/HttpServletResponse.h>

#include <httpModule/dispatcher/HandlerExecutionChain.h>
#include <httpModule/controllers/PlantController.h>

#include <httpModule/interceptors/JsonHandlerInterceptor.h>
#include <httpModule/controllers/ProgramController.h>
#include <httpModule/controllers/GardenController.h>

namespace httpModule {

//dispatcher and webserver glue
class DispatcherHandler : public RequestHandler {
public:
	DispatcherHandler(std::shared_ptr<Http::DispatcherServlet> dispatcherServlet):
		_dispatcher(dispatcherServlet)
    {
    }

    bool canHandle(HTTPMethod , String ) override  {
        return true;
    }

    bool canUpload(String ) override  {
    	Serial.println("?????????not suppose to be called: canUpload() in DispatcherHandler : public RequestHandler ??????????");
        return false;
    }

    std::shared_ptr<Http::HttpServletRequest> _createHttpServletRequest(ESP8266WebServer& server){
        //from what i understood from the text the RequestArgument with the key of "plain" is where the json data is going
        String requestBody="";
        if(server.hasArg("plain"))
        	requestBody = server.arg("plain");
       // Serial.println ( " before new HttpServletRequest" );
        //translating the method:
        Http::HTTPMethod ourMethod;
        HTTPMethod serverMethod = server.method();
        switch (serverMethod){
        	case HTTP_ANY: ourMethod = Http::HTTP_ANY; break;
        	case HTTP_GET: ourMethod = Http::HTTP_GET; break;
        	case HTTP_POST: ourMethod = Http::HTTP_POST; break;
        	case HTTP_PUT: ourMethod = Http::HTTP_PUT; break;
        	case HTTP_PATCH: ourMethod = Http::HTTP_PATCH; break;
        	case HTTP_DELETE: ourMethod = Http::HTTP_DELETE; break;
        	case HTTP_OPTIONS: ourMethod = Http::HTTP_OPTIONS; break;
        	default:
        		Serial.println("___ERROR: unrecognized http method");
        		return nullptr;
        		break;
        }
        return std::make_shared<Http::HttpServletRequest>(requestBody, ourMethod, server.uri());
    }

    void printServletRequest(Http::HttpServletRequest& req){
    	Serial.println("The Request parameters:");
    	Serial.println("url: " + req.url);
    	Serial.print("Method: ");
    	Serial.println((req.httPMethod == Http::HTTP_GET ? "GET" : "POST"));
    	Serial.println("Body: " + req.requestBody);
    	Serial.println("url tokens:");
    	for (unsigned int i = 0; i < req.urlTokens.size(); ++i) {
    		Serial.print("	-token: ");
    		Serial.println(req.urlTokens[i]);
    	}
    }

    bool handle(ESP8266WebServer& server, HTTPMethod method, String ) override {
    	if(method == HTTPMethod::HTTP_OPTIONS){
    	    	Serial.println("inside the default HTTPMethod::HTTP_OPTIONS handling function");
    	    	server.sendHeader("Allow", "OPTIONS, GET, HEAD, POST");
    	    	server.sendHeader("Access-Control-Allow-Origin", "*");//
    	    	server.sendHeader("Access-Control-Allow-Headers", "content-type");
    	    	// this->sendHeader("Access-Control-Allow-Methods", "OPTIONS, GET, HEAD, POST");
    	    	// this->sendHeader("Content-Type", "application/json");
    	    	server.send(200);
    	    }else{
    	    	server.sendHeader("Allow", "OPTIONS, GET, HEAD, POST");
				server.sendHeader("Access-Control-Allow-Origin", "*");//
				server.sendHeader("Access-Control-Allow-Headers", "content-type");
    		//	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

    		//	Serial.println ( "Calling _createHttpServletRequest()" );
    			std::shared_ptr<Http::HttpServletRequest> httpServletRequest = _createHttpServletRequest(server);
    			if(httpServletRequest == nullptr){
    				Serial.printf("______ Recieved unknown http method");
    				server.sendHeader("Allow", "OPTIONS, GET, HEAD, POST");
    				server.send(405);
    				return false;
    			}

    		//	printServletRequest(*httpServletRequest);

    	//		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
    	//		Serial.println ( "Dispatching the request Calling FispatcherServlet.dispatch(HttpServletRequest)" );
    			std::shared_ptr<Http::HttpServletResponse> httpServletResponse = _dispatcher->dispatch(*httpServletRequest);


    			Serial.println ( "~~~~The request we got:");
    			printServletRequest(*httpServletRequest);
    			Serial.println ( "~~~~The response we send:");
    			Serial.print("code: ");
    			Serial.println(httpServletResponse->_httpCode);
    			Serial.print("content_type: ");
    			Serial.println(httpServletResponse->content_type);
    			Serial.print("body: ");
    			Serial.println(httpServletResponse->content);
    			server.send(httpServletResponse->_httpCode, httpServletResponse->content_type, httpServletResponse->content );

    			//here we need cleanup
    			Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
    	    }

        return true;
    }


    void upload(ESP8266WebServer& , String , HTTPUpload& ) override {
    	Serial.println("?????????not suppose to be called: upload() in DispatcherHandler : public RequestHandler ??????????");
    }

protected:
    std::shared_ptr<Http::DispatcherServlet> _dispatcher;
};

std::shared_ptr<ESP8266WebServer> ESP8266WebServerCreator(std::shared_ptr<Http::DispatcherServlet> dispatcherServlet){
	std::shared_ptr<ESP8266WebServer> webServer = std::make_shared<ESP8266WebServer>();
	DispatcherHandler* dispatcherHandler = new DispatcherHandler(dispatcherServlet);
	webServer->addHandler(dispatcherHandler);
	return (webServer);
}

std::shared_ptr<Http::DispatcherServlet> dispatcherServletCreator(std::vector<std::shared_ptr<Http::HandlerExecutionChain>> excecutionChains){
	return std::make_shared<Http::DispatcherServlet>(excecutionChains);
}

std::shared_ptr<Http::HandlerExecutionChain> plantExcecutionChainCreator(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork, std::shared_ptr<Http::JsonHandlerInterceptor> jsonInterceptor){
	std::shared_ptr<Http::HandlerExecutionChain> plantExceChain = std::make_shared<Http::HandlerExecutionChain>(std::make_shared<Http::PlantController>(unitOfWork));
	plantExceChain->addInterceptor(jsonInterceptor);
	return plantExceChain;
}

std::shared_ptr<Http::HandlerExecutionChain> programExcecutionChainCreator(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork, std::shared_ptr<Http::JsonHandlerInterceptor> jsonInterceptor){
	std::shared_ptr<Http::HandlerExecutionChain> programExceChain = std::make_shared<Http::HandlerExecutionChain>(std::make_shared<Http::ProgramController>(unitOfWork));
	programExceChain->addInterceptor(jsonInterceptor);
	return programExceChain;
}

std::shared_ptr<Http::HandlerExecutionChain> gardenExcecutionChainCreator(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork, std::shared_ptr<Http::JsonHandlerInterceptor> jsonInterceptor, std::shared_ptr<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::APIMappingFile>, mycereal::JsonLoadArchive<DAL::APIMappingFile>>> jsonAPISerializationService){
	std::shared_ptr<Http::HandlerExecutionChain> gardenExceChain = std::make_shared<Http::HandlerExecutionChain>(std::make_shared<Http::GardenController>(unitOfWork, jsonAPISerializationService));
	gardenExceChain->addInterceptor(jsonInterceptor);
	return gardenExceChain;
}

std::shared_ptr<Http::JsonHandlerInterceptor> jsonHandlerInterceptorCreator(std::shared_ptr<GardenModel::JsonGardenVisitor> jsonGardenVisitor){
	return std::make_shared<Http::JsonHandlerInterceptor>(jsonGardenVisitor);
}

std::shared_ptr<GardenModel::JsonGardenVisitor> jsonGardenVisitorCreator(std::shared_ptr<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::APIMappingFile>, mycereal::JsonLoadArchive<DAL::APIMappingFile>>> jsonAPISerializationService){
	return std::make_shared<GardenModel::JsonGardenVisitor>(jsonAPISerializationService);
}


class httpModule : public MF::ModuleBase{
public:
	httpModule() {
#ifdef DEBUG_MY_CODE
		Serial.println("httpModule CTOR");
#endif
		 }
	virtual ~httpModule(){
#ifdef DEBUG_MY_CODE
		Serial.println("httpModule DTOR");
#endif
		}

	void start(std::shared_ptr<cntnr::Container> container){
#ifdef DEBUG_MY_CODE
		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
		Serial.println("httpModule start");
#endif
		container->registerType<Http::DispatcherServlet>(&dispatcherServletCreator, true);
		container->registerType<ESP8266WebServer>(&ESP8266WebServerCreator);

		container->registerType<Http::JsonHandlerInterceptor>(&jsonHandlerInterceptorCreator);
		container->registerType<Http::HandlerExecutionChain>(&plantExcecutionChainCreator);

		container->registerType<GardenModel::JsonGardenVisitor>(&jsonGardenVisitorCreator);

		container->registerType<Http::HandlerExecutionChain>(&programExcecutionChainCreator);
		container->registerType<Http::HandlerExecutionChain>(&gardenExcecutionChainCreator);
	}
};

} /* namespace DAL */

#endif /* HTTPMODULE_HTTPMODULE_H_ */
