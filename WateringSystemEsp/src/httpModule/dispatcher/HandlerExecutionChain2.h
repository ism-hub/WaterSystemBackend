/*
 * HandlerExecutionChain.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_HANDLEREXECUTIONCHAIN2_H_
#define HTTP_HANDLEREXECUTIONCHAIN2_H_

#include <httpModule/controllers/PlantController.h>
#include <httpModule/dispatcher/IController.h>
#include <vector>
#include <memory>

#include <httpModule/dispatcher/IHandlerExecutionChain.h>
#include <httpModule/dispatcher/IHandlerInterceptor.h>

using namespace std;

namespace Http {

template <class AcceptableType>
class HandlerExecutionChain2 : public IHandlerExecutionChain {
public:

	std::shared_ptr<IController<AcceptableType>> _controller;
	vector<std::shared_ptr<IHandlerInterceptor<AcceptableType>>> _handlerInterceptors;

	HandlerExecutionChain2(std::shared_ptr<IController<AcceptableType>> controller) : _controller(controller) {}
	virtual ~HandlerExecutionChain2() {}

	void addInterceptor(std::shared_ptr<IHandlerInterceptor<AcceptableType>> interceptor){
		_handlerInterceptors.push_back(interceptor);
	}

	std::shared_ptr<IController<AcceptableType>> getController() {
		return _controller;
	}

	vector<std::shared_ptr<IHandlerInterceptor<AcceptableType>>>& getInterceptors(){
		return _handlerInterceptors;
	}

	bool canHandle(HttpServletRequest& req) override {
		return _controller->canHandle(req);
	}

	std::shared_ptr<HttpServletResponse> execute(HttpServletRequest& req) override {
		std::shared_ptr<HttpServletResponse> response = std::make_shared<HttpServletResponse>();
		//HandlerInterceptors before handle (every one return bool if returned true than we need to stop the execution)
		Serial.println("Inside HandlerExecutionChain execute");
		for (unsigned int i = 0; i < _handlerInterceptors.size(); i++) {
			if (!_handlerInterceptors[i]->preHandle(req, *response, *_controller)) // if false then the Interceptor handled the response
				return response;
		}

		// call the handler
		std::shared_ptr<AcceptableType> acceptable = _controller->handle(req, *response);
		if (acceptable == NULL) { //error happened inside the controller, and the controller prepared the right httPresponse with the error properties
			return response;
		}

		//HandlerInterceptors post handle
		for (size_t i = 0; i < _handlerInterceptors.size(); i++) {
			_handlerInterceptors[i]->postHandle(req, *response, *_controller,*acceptable);
		}

		return response;
	}
};

} /* namespace Http */

#endif /* HTTP_HANDLEREXECUTIONCHAIN2_H_ */
