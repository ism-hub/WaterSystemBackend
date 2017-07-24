/*
 * HandlerExecutionChain.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_HANDLEREXECUTIONCHAIN_H_
#define HTTP_HANDLEREXECUTIONCHAIN_H_

#include <vector>
#include <HandlerInterceptor.h>
#include <Controller.h>
#include <PlantController.h>

using namespace std;

namespace Http {

class HandlerExecutionChain {
public:

	Controller* _controller;
	vector<HandlerInterceptor*> _handlerInterceptors;

	HandlerExecutionChain(Controller* controller) {
		Serial.println ("CTOR of HandlerExecutionChain called");
		_handlerInterceptors.reserve(10);
		_controller=controller;
	}
	virtual ~HandlerExecutionChain() {
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ HandlerExecutionChain DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	void addInterceptor(HandlerInterceptor* interceptor){
		_handlerInterceptors.push_back(interceptor);
	}

	Controller* getController() {
		Serial.println ("inside the getController function");
		if(_controller == NULL)
			Serial.println ("the controller is null for some reason");
		Serial.println ("returning the controller inside HandlerExecutionChain");
		return _controller;
		//return NULL;
	}

	vector<HandlerInterceptor*>& getInterceptors(){
		return _handlerInterceptors;
	}
};

} /* namespace Http */

#endif /* HTTP_HANDLEREXECUTIONCHAIN_H_ */
