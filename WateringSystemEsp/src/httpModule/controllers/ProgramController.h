/*
 * ProgramController.h
 *
 *  Created on: 11 баеч„ 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_CONTROLLERS_PROGRAMCONTROLLER_H_
#define HTTPMODULE_CONTROLLERS_PROGRAMCONTROLLER_H_

#include <list>
#include <Plant.h>
#include <stdlib_noniso.h>//atoi
#include <DALModule/repositoryPattern/GardenUnitOfWork.h>
#include <GardenAcceptable.h>
#include <Garden.h>
#include <httpModule/dispatcher/IController.h>
#include <httpModule/model/HttpServletRequest.h>
#include <httpModule/model/HttpServletResponse.h>

namespace Http {

class ProgramController : public IController<GardenModel::GardenAcceptable>{

protected:
	std::shared_ptr<DAL::GardenUnitOfWork> _unitOfWork;

public:
	ProgramController(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork): _unitOfWork(unitOfWork) {

	}
	virtual ~ProgramController(){

	}

	bool canHandle(HttpServletRequest& req) {
		if (req.httPMethod == HTTP_GET && req.urlTokens.size() > 0 && req.urlTokens[0] == "programs") {
			return true;
		}
		return false;
	}

	std::shared_ptr<GardenAcceptable> handle(HttpServletRequest& req, HttpServletResponse&) {
		std::shared_ptr<GardenModel::SimpleProgram> program = nullptr;
		if (canHandle(req) && req.urlTokens.size() == 2) { //get a program     Ц GET         /programs/{id}
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
