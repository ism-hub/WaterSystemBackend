/*
 * HandlerInterceptor.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_HANDLERINTERCEPTOR_H_
#define HTTP_HANDLERINTERCEPTOR_H_

#include <HttpServletRequest.h>
#include <HttpServletResponse.h>
#include <Controller.h>//### check to see if i need this
#include <GardenAcceptable.h>

using namespace GardenModel;

namespace Http {

class HandlerInterceptor {
public:
	HandlerInterceptor() {

	}
	virtual ~HandlerInterceptor() {
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ HandlerInterceptor DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	virtual bool preHandle(HttpServletRequest& request, HttpServletResponse& response, Controller& handler) = 0;

	virtual void postHandle(HttpServletRequest& request, HttpServletResponse& response, Controller& handler, GardenAcceptable& model) = 0;
};

} /* namespace Http */

#endif /* HTTP_HANDLERINTERCEPTOR_H_ */
