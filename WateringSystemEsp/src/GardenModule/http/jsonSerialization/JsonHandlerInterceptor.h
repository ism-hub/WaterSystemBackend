/*
 * JsonHandlerInterceptor.h
 *
 *  Created on: 22 ����� 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_JSONHANDLERINTERCEPTOR_H_
#define HTTPMODULE_JSONHANDLERINTERCEPTOR_H_

#include <GardenModule/model/GardenVisitor.h>
#include <HttpFramework/dispatcher/IHandlerInterceptor.h>
#include <HttpFramework/dispatcher/IController.h>
#include <DALFramework/serializationService/JsonSerializationService2.h>
#include <GardenModule/http/jsonSerialization/JsonGardenVisitor.h>

namespace garden {

class JsonHandlerInterceptor : public Http::IHandlerInterceptor<GardenAcceptable> {
public:
	std::shared_ptr<garden::JsonGardenVisitor>	_jsonGardenVisitor;
	JsonHandlerInterceptor(std::shared_ptr<garden::JsonGardenVisitor>	jsonGardenVisitor) : _jsonGardenVisitor(jsonGardenVisitor){
	}

	virtual ~JsonHandlerInterceptor(){
	}

	bool preHandle(Http::HttpServletRequest& , Http::HttpServletResponse& , Http::IController<GardenAcceptable>&) {
		return true;
	}

	void postHandle(Http::HttpServletRequest& , Http::HttpServletResponse& response, Http::IController<GardenAcceptable>& , GardenAcceptable& model)  {
		//convert the object into json string
		String jsonModel = std::move(*std::static_pointer_cast<String>(model.accept(*_jsonGardenVisitor)));
		response.content = jsonModel;
		response.content_type = Http::HttpServletResponse::CONTENT_TYPE_JSON;
		response._httpCode = Http::SC_OK;
	}
};

} /* namespace GardenModel */

#endif /* HTTPMODULE_JSONHANDLERINTERCEPTOR_H_ */
