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

using namespace std;

namespace Http {

class HandlerExecutionChain {
public:

	std::shared_ptr<Controller> _controller;
	vector<std::shared_ptr<HandlerInterceptor>> _handlerInterceptors;

	HandlerExecutionChain(std::shared_ptr<Controller> controller) : _controller(controller) {
		Serial.println ("CTOR of HandlerExecutionChain called");
		_handlerInterceptors.reserve(5);
	}
	virtual ~HandlerExecutionChain() {
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ HandlerExecutionChain DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	void addInterceptor(std::shared_ptr<HandlerInterceptor> interceptor){
		_handlerInterceptors.push_back(interceptor);
	}

	std::shared_ptr<Controller> getController() {
		Serial.println ("inside the getController function");
		if(_controller == NULL)
			Serial.println ("the controller is null for some reason");
		Serial.println ("returning the controller inside HandlerExecutionChain");
		return _controller;
		//return NULL;
	}

	vector<std::shared_ptr<HandlerInterceptor>>& getInterceptors(){
		return _handlerInterceptors;
	}
};

} /* namespace Http */

#endif /* HTTP_HANDLEREXECUTIONCHAIN_H_ */
