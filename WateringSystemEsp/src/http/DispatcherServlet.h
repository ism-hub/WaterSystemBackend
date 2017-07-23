/*
 * DispatcherServlet.h
 *
 *  Created on: 10 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_DISPATCHERSERVLET_H_
#define HTTP_DISPATCHERSERVLET_H_

#include <HttpServletRequest.h>
#include <HttpServletResponse.h>
#include <WateringSystemApiParser.h>
#include <vector>
#include <Controller.h>
#include <HandlerExecutionChain.h>
#include <PlantController.h>
#include <HandlerInterceptor.h>
#include <JsonGardenVisitor.h>
#include <JsonHandlerInterceptor.h>
#include <PlantController.h>


using namespace std;
using namespace GardenModel;

namespace Http {

class DispatcherServlet {
public:

	//typedef HttpServletResponse (*t_name)(HttpServletRequest& request);

	vector<HandlerExecutionChain*>* _handlerExecutionChains=NULL;
	Garden* _garden=NULL;

	DispatcherServlet() {
		Serial.println ("## inside DispatcherServlet CTOR ");
		_handlerExecutionChains = new vector<HandlerExecutionChain*>(1);
	/*	Serial.println ("############# inside DispatcherServlet CTOR ###########");
		_garden = new Garden();
		_handlerExecutionChains  = new vector<HandlerExecutionChain*>(1);

		Serial.println ("############# creating the PlantController ###########");
		PlantController* plantCtrl = new PlantController(*_garden);
		HandlerExecutionChain* handlerExecutionChain = new HandlerExecutionChain(plantCtrl);

		Serial.println ("############# trying to do: handlerExecutionChain->_controller ###########");
		Controller* ctrl = handlerExecutionChain->_controller;

		//adding the interceptor
		JsonGardenVisitor* jsonVisitor = new JsonGardenVisitor();
		JsonHandlerInterceptor* jsonInterceptor = new JsonHandlerInterceptor(*jsonVisitor);
		handlerExecutionChain->addInterceptor(jsonInterceptor);

		_handlerExecutionChains->push_back(handlerExecutionChain);*/
	}
	virtual ~DispatcherServlet() {
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ DispatcherServlet DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	HandlerExecutionChain* getHandlerExecutionChain(HttpServletRequest& request) {
		for (int i = 0; i < _handlerExecutionChains->size(); i++) {

			Serial.println ("doing vector<HandlerExecutionChain*>& handlerExecutionChains = (*_handlerExecutionChains);");
			vector<HandlerExecutionChain*>& handlerExecutionChains = (*_handlerExecutionChains);

			Serial.println ("doing HandlerExecutionChain& handlerExecutionChain = handlerExecutionChains[i];");
			HandlerExecutionChain& handlerExecutionChain = *(handlerExecutionChains[i]);

			Serial.println ("handlerExecutionChain._controller instead of handlerExecutionChain.getController();");
			Controller* ctrl = handlerExecutionChain.getController();

			Serial.println ("checking if he can handle");
			if (ctrl->canHandle(request)){
				Serial.println ("yes we can");
				return (*_handlerExecutionChains)[i];
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
		vector<HandlerInterceptor*>& HandlerInterceptors = *(executionChain->getInterceptors());
		Controller& controller = *(executionChain->getController());

		//HandlerInterceptors before handle (every one return bool if returned true than we need to stop the execution)
		Serial.println ("execute the pre inceptors" );
		for(int i = 0; i<HandlerInterceptors.size();i++){
			if(!HandlerInterceptors[i]->preHandle(request,*response,controller))// if false then the Interceptor handled the response
				return response;
		}

		Serial.println ( "Calling the handler (Controller)" );
		// call the handler
		GardenAcceptable* gardenAcceptable = controller.handle(request,*response);
		if(gardenAcceptable == NULL)//error happened inside the controller, and the controller prepared the right httPresponse with the error properties
			return response;

		Serial.println ( "Calling the HandlerInterceptors post handle" );
		//HandlerInterceptors post handle
		for (int i = 0; i < HandlerInterceptors.size(); i++) {
			HandlerInterceptors[i]->postHandle(request, *response,controller,*gardenAcceptable);
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
