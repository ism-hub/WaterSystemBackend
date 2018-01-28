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
#ifdef DEBUG_MY_CODE
		Serial.println ("DispatcherServlet CTOR ");
#endif

	}

	virtual ~DispatcherServlet() {
#ifdef DEBUG_MY_CODE
		Serial.println("DispatcherServlet DTOR");
#endif
	}

	std::shared_ptr<HandlerExecutionChain> getHandlerExecutionChain(HttpServletRequest& request) {
		for(auto& exeChain : _handlerExecutionChains){
			if(exeChain->canHandle(request))
				return exeChain;
		}
		return nullptr;
	}

	std::shared_ptr<HttpServletResponse> dispatch(HttpServletRequest& request) {
#ifdef DEBUG_MY_CODE
		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
		Serial.println("Dispatching the request");
#endif
		std::shared_ptr<HandlerExecutionChain> executionChain = getHandlerExecutionChain(request);
		if(executionChain == nullptr) {
#ifdef DEBUG_MY_CODE
			Serial.println("We didnt find any handler for the request, returning the default response.");
#endif
			std::shared_ptr<HttpServletResponse> resp = std::make_shared<HttpServletResponse>();
			resp->_httpCode = SC_NOT_FOUND;
			return resp;
		}
#ifdef DEBUG_MY_CODE
		Serial.println ("we found an handler, we can execute it" );
#endif
		std::shared_ptr<HttpServletResponse> response = executionChain->execute(request);
		return response;
	}

};

} /* namespace Http */

#endif /* HTTP_DISPATCHERSERVLET_H_ */
