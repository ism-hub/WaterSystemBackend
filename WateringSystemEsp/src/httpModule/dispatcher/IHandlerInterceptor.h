/*
 * HandlerInterceptor.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_IHANDLERINTERCEPTOR_H_
#define HTTP_IHANDLERINTERCEPTOR_H_

#include <GardenAcceptable.h>
#include <httpModule/dispatcher/IController.h>//### check to see if i need this
#include <httpModule/model/HttpServletRequest.h>
#include <httpModule/model/HttpServletResponse.h>

using namespace GardenModel;

namespace Http {

template <class AcceptableType>
class IHandlerInterceptor {
public:
	IHandlerInterceptor() {
	}
	virtual ~IHandlerInterceptor() {
	}

	virtual bool preHandle(HttpServletRequest& request, HttpServletResponse& response, IController<AcceptableType>& handler) = 0;

	virtual void postHandle(HttpServletRequest& request, HttpServletResponse& response, IController<AcceptableType>& handler, AcceptableType& model) = 0;
};

} /* namespace Http */

#endif /* HTTP_IHANDLERINTERCEPTOR_H_ */
