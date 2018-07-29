/*
 * JsonHandlerInterceptor.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_JSONHANDLERINTERCEPTOR_H_
#define HTTPMODULE_JSONHANDLERINTERCEPTOR_H_

#include <GardenVisitor.h>
#include <httpModule/dispatcher/IHandlerInterceptor.h>
#include <httpModule/dispatcher/IController.h>
#include <DALModule/serializationService/JsonSerializationService2.h>
#include <httpModule/interceptors/JsonGardenVisitor.h>

namespace Http {

class JsonHandlerInterceptor : public IHandlerInterceptor<GardenAcceptable> {
public:
	std::shared_ptr<GardenModel::JsonGardenVisitor>	_jsonGardenVisitor;
	JsonHandlerInterceptor(std::shared_ptr<GardenModel::JsonGardenVisitor>	jsonGardenVisitor) : _jsonGardenVisitor(jsonGardenVisitor){
	}

	virtual ~JsonHandlerInterceptor(){
	}

	bool preHandle(HttpServletRequest& , HttpServletResponse& , IController<GardenAcceptable>&) {
		return true;
	}

	void postHandle(HttpServletRequest& , HttpServletResponse& response, IController<GardenAcceptable>& , GardenAcceptable& model)  {
		//convert the object into json string
		String jsonModel = std::move(*std::static_pointer_cast<String>(model.accept(*_jsonGardenVisitor)));
		response.content = jsonModel;
		response.content_type = HttpServletResponse::CONTENT_TYPE_JSON;
		response._httpCode = SC_OK;
	}
};

} /* namespace GardenModel */

#endif /* HTTPMODULE_JSONHANDLERINTERCEPTOR_H_ */
