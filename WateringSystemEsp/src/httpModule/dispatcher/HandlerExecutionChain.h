/*
 * HandlerExecutionChain.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_HANDLEREXECUTIONCHAIN_H_
#define HTTP_HANDLEREXECUTIONCHAIN_H_

#include <httpModule/dispatcher/Controller.h>
#include <httpModule/controllers/PlantController.h>
#include <httpModule/dispatcher/HandlerInterceptor.h>
#include <vector>
#include <memory>

#include <httpModule/dispatcher/IHandlerExecutionChain.h>

using namespace std;

namespace Http {

class HandlerExecutionChain : public IHandlerExecutionChain {
public:

	std::shared_ptr<Controller> _controller;
	vector<std::shared_ptr<HandlerInterceptor>> _handlerInterceptors;

	HandlerExecutionChain(std::shared_ptr<Controller> controller) : _controller(controller) {
#ifdef DEBUG_MY_CODE
		Serial.println ("HandlerExecutionChain CTOR");
#endif
	}
	virtual ~HandlerExecutionChain() {
#ifdef DEBUG_MY_CODE
		Serial.println("HandlerExecutionChain DTOR");
#endif
	}

	void addInterceptor(std::shared_ptr<HandlerInterceptor> interceptor){
		_handlerInterceptors.push_back(interceptor);
	}

	std::shared_ptr<Controller> getController() {
		return _controller;
	}

	vector<std::shared_ptr<HandlerInterceptor>>& getInterceptors(){
		return _handlerInterceptors;
	}

	bool canHandle(HttpServletRequest& req) override {
		return _controller->canHandle(req);
	}

	std::shared_ptr<HttpServletResponse> execute(HttpServletRequest& req) override {
		std::shared_ptr<HttpServletResponse> response = std::make_shared<HttpServletResponse>();
		//HandlerInterceptors before handle (every one return bool if returned true than we need to stop the execution)
#ifdef DEBUG_MY_CODE
		Serial.println("execute the pre inceptors");
#endif
		for (unsigned int i = 0; i < _handlerInterceptors.size(); i++) {
			if (!_handlerInterceptors[i]->preHandle(req, *response, *_controller)) // if false then the Interceptor handled the response
				return response;
		}
#ifdef DEBUG_MY_CODE
		Serial.println("Calling the handler (Controller)");
#endif
		// call the handler
		std::shared_ptr<GardenAcceptable> gardenAcceptable = _controller->handle(req, *response);
		if (gardenAcceptable == NULL) { //error happened inside the controller, and the controller prepared the right httPresponse with the error properties
#ifdef DEBUG_MY_CODE
			Serial.println("gardenAcceptable is null, maybe a problem in the controller ");
#endif
			return response;
		}

#ifdef DEBUG_MY_CODE
		Serial.println("Calling the HandlerInterceptors post handle");
#endif
		//HandlerInterceptors post handle
		for (size_t i = 0; i < _handlerInterceptors.size(); i++) {
			_handlerInterceptors[i]->postHandle(req, *response, *_controller,*gardenAcceptable);
		}

		return response;
	}
};

} /* namespace Http */

#endif /* HTTP_HANDLEREXECUTIONCHAIN_H_ */
