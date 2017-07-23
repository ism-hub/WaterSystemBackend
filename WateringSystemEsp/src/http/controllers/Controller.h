/*
 * Controller.h
 *
 *  Created on: 16 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_CONTROLLERS_CONTROLLER_H_
#define HTTP_CONTROLLERS_CONTROLLER_H_

#include <HttpServletRequest.h>
#include <HttpServletResponse.h>
#include <GardenAcceptable.h>

namespace Http {

class Controller {
public:

	Controller(){

	}
	virtual ~Controller(){
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ Controller DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	virtual bool canHandle(HttpServletRequest& req)=0;
	virtual GardenModel::GardenAcceptable* handle(HttpServletRequest& req, HttpServletResponse& response)=0;
};

} /* namespace Http */

#endif /* HTTP_CONTROLLERS_CONTROLLER_H_ */
