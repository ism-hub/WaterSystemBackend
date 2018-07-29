/*
 * Controller.h
 *
 *  Created on: 16 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_CONTROLLERS_CONTROLLER_H_
#define HTTPMODULE_CONTROLLERS_CONTROLLER_H_

#include <GardenAcceptable.h>
#include <httpModule/model/HttpServletRequest.h>
#include <httpModule/model/HttpServletResponse.h>

namespace Http {

class Controller {
public:

	Controller(){
	}
	virtual ~Controller(){
	}

	virtual bool canHandle(HttpServletRequest& req)=0;
	virtual std::shared_ptr<GardenModel::GardenAcceptable> handle(HttpServletRequest& req, HttpServletResponse& response)=0;
};

} /* namespace Http */

#endif /* HTTPMODULE_CONTROLLERS_CONTROLLER_H_ */
