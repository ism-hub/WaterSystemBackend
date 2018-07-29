/*
 * serializationHandlerInterceptor.h
 *
 *  Created on: 28 αιεμι 2018
 *      Author: IsM
 */

#ifndef HTTPMODULE_INTERCEPTORS_VISITORHANDLERINTERCEPTOR_H_
#define HTTPMODULE_INTERCEPTORS_VISITORHANDLERINTERCEPTOR_H_

#include <memory>

#include <httpModule/model/HttpServletRequest.h>
#include <httpModule/model/HttpServletResponse.h>
#include <httpModule/dispatcher/IController.h>
#include <httpModule/dispatcher/IHandlerInterceptor.h>

namespace Http {

template <class Visitor, class Acceptable>
class visitorHandlerInterceptor : public IHandlerInterceptor<Acceptable> {
public:
	std::shared_ptr<Visitor>	visitor;
	visitorHandlerInterceptor(std::shared_ptr<Visitor> visitor) :  visitor(visitor) {}
	virtual ~visitorHandlerInterceptor() {}

	bool preHandle(HttpServletRequest& , HttpServletResponse& , IController<Acceptable>&){
		return true;
	}

	void postHandle(HttpServletRequest& , HttpServletResponse& response, IController<Acceptable>&, Acceptable& model) {
		//convert the object into json string
		String serialized = std::move(*std::static_pointer_cast<String>(model.accept(*visitor)));
		response.content = serialized;
		response.content_type = HttpServletResponse::CONTENT_TYPE_JSON;
		response._httpCode = SC_OK;
	}
};

} /* namespace Http */

#endif /* HTTPMODULE_INTERCEPTORS_VISITORHANDLERINTERCEPTOR_H_ */
