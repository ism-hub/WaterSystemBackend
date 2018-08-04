/*
 * HandlerInterceptor.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_IHANDLERINTERCEPTOR_H_
#define HTTP_IHANDLERINTERCEPTOR_H_

#include <HttpFramework/dispatcher/IController.h>//### check to see if i need this
#include <HttpFramework/model/HttpServletRequest.h>
#include <HttpFramework/model/HttpServletResponse.h>

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
