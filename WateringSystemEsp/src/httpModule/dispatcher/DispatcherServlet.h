/*
 * DispatcherServlet.h
 *
 *  Created on: 10 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_DISPATCHERSERVLET_H_
#define HTTP_DISPATCHERSERVLET_H_

#include <memory>
#include <httpModule/dispatcher/Controller.h>
#include <httpModule/controllers/PlantController.h>
#include <httpModule/controllers/PlantController.h>
#include <httpModule/interceptors/JsonHandlerInterceptor.h>
#include <httpModule/WateringSystemApiParser.h>
#include <httpModule/model/HttpServletRequest.h>
#include <httpModule/model/HttpServletResponse.h>
#include <vector>
#include <httpModule/dispatcher/HandlerExecutionChain.h>
#include <httpModule/dispatcher/HandlerInterceptor.h>
//#include <visitors/JsonGardenVisitor.obsolete>


using namespace std;
using namespace GardenModel;

namespace Http {

class DispatcherServlet {
public:
	vector<std::shared_ptr<HandlerExecutionChain>> _handlerExecutionChains;

	DispatcherServlet(vector<std::shared_ptr<HandlerExecutionChain>> exeChains = {}) : _handlerExecutionChains(exeChains) {
		Serial.println ("DispatcherServlet CTOR ");
	}

	virtual ~DispatcherServlet() {
		Serial.println("DispatcherServlet DTOR");
	}

	HandlerExecutionChain* getHandlerExecutionChain(HttpServletRequest& request) {
		for (int i = 0; i < _handlerExecutionChains.size(); i++) {
			HandlerExecutionChain& handlerExecutionChain = *_handlerExecutionChains[i];

			Serial.println (" handlerExecutionChain.getController();");
			std::shared_ptr<Controller> ctrl = handlerExecutionChain.getController();

			Serial.println ("checking if he can handle");
			if (ctrl->canHandle(request)){
				Serial.println ("	- yes we can");
				return &handlerExecutionChain;
			}
		}
		return NULL;
	}



	HttpServletResponse* dispatch(HttpServletRequest& request) {
		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
		Serial.println ("Creating the response new HttpServletResponse()" );
		HttpServletResponse* response = new HttpServletResponse();////### make sure you are freeing this thing

		//ParsedRequest parsedRequest = parser.parse(request);

		Serial.println("Finding the execution chain for the request");
		HandlerExecutionChain* executionChain = getHandlerExecutionChain(request);

		Serial.println ("chcking if executionChain is null (if we found handler for the url)" );
		if(executionChain == NULL) {
			Serial.println ("yes it is null ");
			response->_httpCode = SC_NOT_FOUND;
			return response;
		}

		Serial.println ("we found an handler, we can continue" );

		//execute the chain
		vector<std::shared_ptr<HandlerInterceptor>>& handlerInterceptors = executionChain->getInterceptors();
		Controller& controller = *(executionChain->getController());

		//HandlerInterceptors before handle (every one return bool if returned true than we need to stop the execution)
		Serial.println ("execute the pre inceptors" );
		for(int i = 0; i<handlerInterceptors.size() ;i++){
			if(!handlerInterceptors[i]->preHandle(request,*response,controller))// if false then the Interceptor handled the response
				return response;
		}

		Serial.println ( "Calling the handler (Controller)" );
		// call the handler
		GardenAcceptable* gardenAcceptable = controller.handle(request,*response);
		if(gardenAcceptable == NULL){//error happened inside the controller, and the controller prepared the right httPresponse with the error properties
			Serial.println ( "gardenAcceptable is null, maybe a problem in the controller " );
			return response;
		}
		Serial.println ( "we got our GardenAcceptable, now we know its a plant so we printing it values-" );
		Plant* plant = (Plant*)gardenAcceptable;
		Serial.print("Plant.sprinkler._id - ");
		Serial.println(plant->_sprinkler->_id);
		Serial.println("Plant.sprinkler._name - " + plant->_sprinkler->_name);
		Serial.print("Plant.sprinkler._name - ");
		Serial.println(plant->_sprinkler->_status == Sprinkler::On ? "On" : "Off");

		Serial.println ( "Calling the HandlerInterceptors post handle" );
		//HandlerInterceptors post handle
		for (int i = 0; i < handlerInterceptors.size(); i++) {
			handlerInterceptors[i]->postHandle(request, *response,controller,*gardenAcceptable);
		}

		Serial.println ( "Our response is:" );
		Serial.println ( "HttpCode:" + response->_httpCode);
		Serial.println ( "Content:" + response->content);
		Serial.println ( "content_type:" + response->content_type);
		//finished
		return response;
	}


protected:
	//WateringSystemApiParser parser;
};

} /* namespace Http */

#endif /* HTTP_DISPATCHERSERVLET_H_ */
