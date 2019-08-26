/*
 * JsonHandlerInterceptor.h
 *
 *  Created on: 22 ����� 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_JSONHANDLERINTERCEPTOR_H_
#define HTTPMODULE_JSONHANDLERINTERCEPTOR_H_

#include <GardenModule/model/GardenVisitor.h>
#include <HttpFramework.hpp>
#include <DALFramework/serializationService/JsonSerializationService2.h>
#include <GardenModule/http/jsonSerialization/JsonGardenVisitor.h>

namespace garden {

class JsonHandlerInterceptor : public Http::IHandlerInterceptor<GardenAcceptable, String> {
public:
	std::shared_ptr<garden::JsonGardenVisitor>	_jsonGardenVisitor;
	JsonHandlerInterceptor(std::shared_ptr<garden::JsonGardenVisitor>	jsonGardenVisitor) : _jsonGardenVisitor(jsonGardenVisitor){
	}

	virtual ~JsonHandlerInterceptor(){
	}

	bool preHandle(Http::HttpServletRequest<String>& , Http::HttpServletResponse<String>& , Http::IController<GardenAcceptable, String>&) {
		return true;
	}

	void postHandle(Http::HttpServletRequest<String>& , Http::HttpServletResponse<String>& response, Http::IController<GardenAcceptable, String>& , GardenAcceptable& model)  {
		//convert the object into json string
		String jsonModel = std::move(*std::static_pointer_cast<String>(model.accept(*_jsonGardenVisitor)));
		response.content = jsonModel;
		response.content_type = Http::HTTP_CONTENT_TYPE_JSON;
		response._httpCode = Http::HttpCode::SC_OK;
	}
};

} /* namespace GardenModel */

#endif /* HTTPMODULE_JSONHANDLERINTERCEPTOR_H_ */
