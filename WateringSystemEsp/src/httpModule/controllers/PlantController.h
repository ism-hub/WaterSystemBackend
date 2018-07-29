/*
 * SprinklerController.h
 *
 *  Created on: 13 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_CONTROLLERS_PLANTCONTROLLER_H_
#define HTTPMODULE_CONTROLLERS_PLANTCONTROLLER_H_

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

class PlantController : public IController<GardenModel::GardenAcceptable> {

protected:
	std::shared_ptr<DAL::GardenUnitOfWork> _unitOfWork;

public:

	PlantController(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork): _unitOfWork(unitOfWork) {
	}

	virtual ~PlantController() {
#ifdef DEBUG_MY_CODE
		Serial.println("PlantController DTOR");
#endif

	}

	bool canHandle(HttpServletRequest& req) {
		if(req.httPMethod == HTTP_GET && req.urlTokens.size() > 0 && req.urlTokens[0] == "plants"){
#ifdef DEBUG_MY_CODE
			Serial.println ("Plant controller can handle the request." );
#endif
			return true;
		}
		return false;
	}

	std::shared_ptr<GardenAcceptable> handle(HttpServletRequest& req, HttpServletResponse&) {
		std::shared_ptr<Plant> plant = nullptr;
		if(canHandle(req) && req.urlTokens.size() == 2){///GET PLANTS_ID
			//get parameters
			int id=atoi(req.urlTokens[1].c_str());
			//call the correct function
			plant = _unitOfWork->Plants().getById(id);
#ifdef DEBUG_MY_CODE
			Serial.println ("Done handling the request.");
#endif

		}
		return plant;
	}

	/*list<Sprinkler> getSprinklers() {

	}*/
};

} /* namespace Http */

#endif /* HTTPMODULE_CONTROLLERS_PLANTCONTROLLER_H_ */
