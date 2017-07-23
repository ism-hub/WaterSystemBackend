/*
 * JsonHandlerInterceptor.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_JSONHANDLERINTERCEPTOR_H_
#define HTTP_JSONHANDLERINTERCEPTOR_H_

#include <HandlerInterceptor.h>
#include <GardenVisitor.h>

namespace Http {

class JsonHandlerInterceptor: public HandlerInterceptor {
public:
	GardenVisitor& _jsonGardenVisitor;
	JsonHandlerInterceptor(GardenVisitor& jsonGardenVisitor) : _jsonGardenVisitor(jsonGardenVisitor){

	}
	virtual ~JsonHandlerInterceptor(){
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ JsonHandlerInterceptor DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	bool preHandle(HttpServletRequest& request, HttpServletResponse& response, Controller& handler){
		return true;
	}

	void postHandle(HttpServletRequest& request, HttpServletResponse& response, Controller& handler, GardenAcceptable& model) {
		String jsonModel = *(String*)model.accept(_jsonGardenVisitor);
		response.content = jsonModel;
	}


};

} /* namespace GardenModel */

#endif /* HTTP_JSONHANDLERINTERCEPTOR_H_ */
