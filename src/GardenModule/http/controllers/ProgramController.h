/*
 * ProgramController.h
 *
 *  Created on: 11 ����� 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_CONTROLLERS_PROGRAMCONTROLLER_H_
#define HTTPMODULE_CONTROLLERS_PROGRAMCONTROLLER_H_

#include <list>
#include <Plant.h>
#include <stdlib_noniso.h>//atoi
#include <GardenModule/DAL/repositoryPattern/GardenUnitOfWork.h>
#include <GardenAcceptable.h>
#include <Garden.h>
#include <HttpFramework.hpp>

namespace garden {

class ProgramController : public Http::IController<garden::GardenAcceptable, String>{

protected:
	std::shared_ptr<GardenUnitOfWork> _unitOfWork;

public:
	ProgramController(std::shared_ptr<GardenUnitOfWork> unitOfWork): _unitOfWork(unitOfWork) {

	}
	 ~ProgramController(){

	}

	bool canHandle(Http::HttpServletRequest<String>& req) {
		if (req.httPMethod == HTTP_GET && req.urlTokens.size() > 0 && req.urlTokens[0] == String(F("programs"))) {
			return true;
		}
		return false;
	}

	std::shared_ptr<GardenAcceptable> handle(Http::HttpServletRequest<String>& req, Http::HttpServletResponse<String>&) {
		std::shared_ptr<garden::SimpleProgram> program = nullptr;
		if (canHandle(req) && req.urlTokens.size() == 2) { //get a program     � GET         /programs/{id}
		//get parameters
			int id = atoi(req.urlTokens[1].c_str());
			//call the correct function
			program = _unitOfWork->Programs().getById(id);
		}
		return program;
	}

};

} /* namespace Http */

#endif /* HTTPMODULE_CONTROLLERS_PROGRAMCONTROLLER_H_ */
