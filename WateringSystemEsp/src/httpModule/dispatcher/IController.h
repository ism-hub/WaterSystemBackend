/*
 * Controller.h
 *
 *  Created on: 16 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_ICONTROLLERS_CONTROLLER_H_
#define HTTPMODULE_ICONTROLLERS_CONTROLLER_H_

#include <GardenAcceptable.h>
#include <httpModule/model/HttpServletRequest.h>
#include <httpModule/model/HttpServletResponse.h>

namespace Http {

template <class AcceptableType>
class IController {
public:

	IController(){
	}
	virtual ~IController(){
	}

	virtual bool canHandle(HttpServletRequest& req)=0;
	virtual std::shared_ptr<AcceptableType> handle(HttpServletRequest& req, HttpServletResponse& response)=0;
};

} /* namespace Http */

#endif /* HTTPMODULE_ICONTROLLERS_CONTROLLER_H_ */
