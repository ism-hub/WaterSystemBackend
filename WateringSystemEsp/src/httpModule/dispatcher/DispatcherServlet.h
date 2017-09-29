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

	std::shared_ptr<HandlerExecutionChain> getHandlerExecutionChain(HttpServletRequest& request) {
		for(auto& exeChain : _handlerExecutionChains){
			if(exeChain->canHandle(request))
				return exeChain;
		}
		return nullptr;
	}

	std::shared_ptr<HttpServletResponse> dispatch(HttpServletRequest& request) {
		Serial.println ("Inside dispatch(HttpServletRequest& request)" );
		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

		Serial.println("Finding the execution chain for the request");
		std::shared_ptr<HandlerExecutionChain> executionChain = getHandlerExecutionChain(request);

		Serial.println ("chcking if executionChain is not null (if we found handler for the url)" );
		if(executionChain == nullptr) {
			Serial.println ("execution chain is null, sending default response");
			std::shared_ptr<HttpServletResponse> resp = std::make_shared<HttpServletResponse>();
			resp->_httpCode = SC_NOT_FOUND;
			return resp;
		}

		Serial.println ("we found an handler, we can execute it" );
		std::shared_ptr<HttpServletResponse> response = executionChain->execute(request);

		Serial.println ( "Our response is:" );
		Serial.println ( "HttpCode:" + response->_httpCode);
		Serial.println ( "Content:" + response->content);
		Serial.println ( "content_type:" + response->content_type);
		//finished
		return response;
	}

};

} /* namespace Http */

#endif /* HTTP_DISPATCHERSERVLET_H_ */
