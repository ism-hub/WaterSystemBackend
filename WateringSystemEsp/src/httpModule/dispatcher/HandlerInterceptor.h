/*
 * HandlerInterceptor.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_HANDLERINTERCEPTOR_H_
#define HTTP_HANDLERINTERCEPTOR_H_

#include <GardenAcceptable.h>
#include <httpModule/dispatcher/Controller.h>//### check to see if i need this
#include <httpModule/model/HttpServletRequest.h>
#include <httpModule/model/HttpServletResponse.h>

using namespace GardenModel;

namespace Http {

class HandlerInterceptor {
public:
	HandlerInterceptor() {

	}
	virtual ~HandlerInterceptor() {
#ifdef DEBUG_MY_CODE
		Serial.println("HandlerInterceptor DTOR");
#endif
	}

	virtual bool preHandle(HttpServletRequest& request, HttpServletResponse& response, Controller& handler) = 0;

	virtual void postHandle(HttpServletRequest& request, HttpServletResponse& response, Controller& handler, GardenAcceptable& model) = 0;
};

} /* namespace Http */

#endif /* HTTP_HANDLERINTERCEPTOR_H_ */
