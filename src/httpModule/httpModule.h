/*
 * httpModule.h
 *
 *  Created on: 23 ����� 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_HTTPMODULE_H_
#define HTTPMODULE_HTTPMODULE_H_

#include <vector>
#include <memory>
#include <ModuleFramework/ModuleFramework.hpp>

#include <HttpFramework/dispatcher/DispatcherServlet.h>
#include <ESP8266WebServer.h>

#include <HttpFramework/model/HttpServletRequest.h>
#include <HttpFramework/model/HttpServletResponse.h>

#include <GardenModule/http/controllers/PlantController.h>

#include <GardenModule/http/jsonSerialization/JsonHandlerInterceptor.h>
//#include <GardenModule/http/controllers/ProgramController.h>
#include <GardenModule/http/controllers/GardenController.h>

#include <HttpFramework/dispatcher/HandlerExecutionChain2.h>

#include <GardenModule/http/jsonSerialization/GardenRESTSerializationService.h>

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
    //	Serial.println("?????????not suppose to be called: canUpload() in DispatcherHandler : public RequestHandler ??????????");
        return false;
    }

    std::shared_ptr<Http::HttpServletRequest> _createHttpServletRequest(ESP8266WebServer& server){
        //from what i understood from the text the RequestArgument with the key of "plain" is where the json data is going
        String requestBody="";
        if(server.hasArg(F("plain")))
        	requestBody = server.arg(F("plain"));
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
        		Serial.println(F("___ERROR: unrecognized http method"));
        		return nullptr;
        		break;
        }
        return std::make_shared<Http::HttpServletRequest>(requestBody, ourMethod, server.uri());
    }

//    void printServletRequest(Http::HttpServletRequest& req){
//    	Serial.println("The Request parameters:");
//    	Serial.println("url: " + req.url);
//    	Serial.print("Method: ");
//    	Serial.println((req.httPMethod == Http::HTTP_GET ? "GET" : "POST"));
//    	Serial.println("Body: " + req.requestBody);
//    	Serial.println("url tokens:");
//    	for (unsigned int i = 0; i < req.urlTokens.size(); ++i) {
//    		Serial.print("	-token: ");
//    		Serial.println(req.urlTokens[i]);
//    	}
//    }

    bool handle(ESP8266WebServer& server, HTTPMethod method, String ) override {
    	if(method == HTTPMethod::HTTP_OPTIONS){
    	//    	Serial.println("inside the default HTTPMethod::HTTP_OPTIONS handling function");
    	    	server.sendHeader(F("Allow"), F("OPTIONS, GET, HEAD, POST"));
    	    	server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));//
    	    	server.sendHeader(F("Access-Control-Allow-Headers"), F("content-type"));
    	    	// this->sendHeader("Access-Control-Allow-Methods", "OPTIONS, GET, HEAD, POST");
    	    	// this->sendHeader("Content-Type", "application/json");
    	    	server.send(200);
    	    }else{
    	    	server.sendHeader(F("Allow"), F("OPTIONS, GET, HEAD, POST"));
				server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));//
				server.sendHeader(F("Access-Control-Allow-Headers"), F("content-type"));
    		//	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

    		//	Serial.println ( "Calling _createHttpServletRequest()" );
    			std::shared_ptr<Http::HttpServletRequest> httpServletRequest = _createHttpServletRequest(server);
    			if(httpServletRequest == nullptr){
    				Serial.println(F("______ Recieved unknown http method"));
    				server.sendHeader(F("Allow"), F("OPTIONS, GET, HEAD, POST"));
    				server.send(405);
    				return false;
    			}

    		//	printServletRequest(*httpServletRequest);

    	//		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
    	//		Serial.println ( "Dispatching the request Calling FispatcherServlet.dispatch(HttpServletRequest)" );
    			std::shared_ptr<Http::HttpServletResponse> httpServletResponse = _dispatcher->dispatch(*httpServletRequest);


//    			Serial.println ( "~~~~The request we got:");
//    			printServletRequest(*httpServletRequest);
//    			Serial.println ( "~~~~The response we send:");
//    			Serial.print("code: ");
//    			Serial.println(httpServletResponse->_httpCode);
//    			Serial.print("content_type: ");
//    			Serial.println(httpServletResponse->content_type);
//    			Serial.print("body: ");
//    			Serial.println(httpServletResponse->content);
    			server.send(httpServletResponse->_httpCode, httpServletResponse->content_type, httpServletResponse->content );

    			//here we need cleanup
    			Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
    	    }

        return true;
    }


    void upload(ESP8266WebServer& , String , HTTPUpload& ) override {
    //	Serial.println("?????????not suppose to be called: upload() in DispatcherHandler : public RequestHandler ??????????");
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

std::shared_ptr<Http::DispatcherServlet> dispatcherServletCreator(std::vector<std::shared_ptr<Http::IHandlerExecutionChain>> excecutionChains){
	return std::make_shared<Http::DispatcherServlet>(excecutionChains);
}

//std::shared_ptr<Http::IHandlerExecutionChain> plantExcecutionChainCreator(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork, std::shared_ptr<Http::JsonHandlerInterceptor> jsonInterceptor){
//	typedef Http::HandlerExecutionChain2<GardenModel::GardenAcceptable> exeChainType;
//	std::shared_ptr<exeChainType> plantExceChain = std::make_shared<exeChainType>(std::make_shared<Http::PlantController>(unitOfWork));
//	plantExceChain->addInterceptor(jsonInterceptor);
//	return plantExceChain;
//}

//std::shared_ptr<Http::IHandlerExecutionChain> programExcecutionChainCreator(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork, std::shared_ptr<Http::JsonHandlerInterceptor> jsonInterceptor){
//	typedef Http::HandlerExecutionChain2<GardenModel::GardenAcceptable> exeChainType;
//	std::shared_ptr<exeChainType> programExceChain = std::make_shared<exeChainType>(std::make_shared<Http::ProgramController>(unitOfWork));
//	programExceChain->addInterceptor(jsonInterceptor);
//	return programExceChain;
//}

std::shared_ptr<Http::IHandlerExecutionChain> gardenExcecutionChainCreator(std::shared_ptr<garden::GardenUnitOfWork> unitOfWork, std::shared_ptr<garden::GardenRESTSerializationService> gardenRESTSerializationService){
	typedef Http::HandlerExecutionChain2<garden::GardenAcceptable> exeChainType;

	auto jsonGardenVisitor = std::make_shared<garden::JsonGardenVisitor>(gardenRESTSerializationService);
	auto jsonHandlerInterceptor = std::make_shared<garden::JsonHandlerInterceptor>(jsonGardenVisitor);

	std::shared_ptr<exeChainType> gardenExceChain = std::make_shared<exeChainType>(std::make_shared<garden::GardenController>(unitOfWork, gardenRESTSerializationService));
	gardenExceChain->addInterceptor(jsonHandlerInterceptor);
	return gardenExceChain;
}

//std::shared_ptr<Http::JsonHandlerInterceptor> jsonHandlerInterceptorCreator(std::shared_ptr<GardenModel::JsonGardenVisitor> jsonGardenVisitor){
//	return std::make_shared<Http::JsonHandlerInterceptor>(jsonGardenVisitor);
//}

//std::shared_ptr<GardenModel::JsonGardenVisitor> jsonGardenVisitorCreator(std::shared_ptr<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::APIMappingFile>, mycereal::JsonLoadArchive<DAL::APIMappingFile>>> jsonAPISerializationService){
//	return std::make_shared<GardenModel::JsonGardenVisitor>(jsonAPISerializationService);
//}


class httpModule : public MF::ModuleBase{
public:
	httpModule() {
		 }
	 ~httpModule(){
		}

	void start(std::shared_ptr<cntnr::Container> container){
	//	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	//	Serial.println("httpModule start");
		container->registerType<Http::DispatcherServlet>(&dispatcherServletCreator, true);
		container->registerType<ESP8266WebServer>(&ESP8266WebServerCreator);

		//container->registerType<Http::JsonHandlerInterceptor>(&jsonHandlerInterceptorCreator);
		//container->registerType<Http::IHandlerExecutionChain>(&plantExcecutionChainCreator);

		//container->registerType<GardenModel::JsonGardenVisitor>(&jsonGardenVisitorCreator);

		//container->registerType<Http::IHandlerExecutionChain>(&programExcecutionChainCreator);
		container->registerType<Http::IHandlerExecutionChain>(&gardenExcecutionChainCreator);
	//	Serial.println("end Inside httpModule start ");
	}
};

} /* namespace DAL */

#endif /* HTTPMODULE_HTTPMODULE_H_ */
