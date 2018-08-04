/*
 * SprinklerController.h
 *
 *  Created on: 13 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTPMODULE_CONTROLLERS_PLANTCONTROLLER_H_
#define HTTPMODULE_CONTROLLERS_PLANTCONTROLLER_H_

#include <list>
#include <GardenModule/model/Plant.h>
#include <stdlib_noniso.h>//atoi
#include <GardenModule/DAL/repositoryPattern/GardenUnitOfWork.h>
#include <GardenModule/model/GardenAcceptable.h>
#include <GardenModule/model/Garden.h>
#include <HttpFramework/dispatcher/IController.h>
#include <HttpFramework/model/HttpServletRequest.h>
#include <HttpFramework/model/HttpServletResponse.h>




namespace garden {

class PlantController : public Http::IController<garden::GardenAcceptable> {

protected:
	std::shared_ptr<garden::GardenUnitOfWork> _unitOfWork;

public:

	PlantController(std::shared_ptr<garden::GardenUnitOfWork> unitOfWork): _unitOfWork(unitOfWork) {
	}

	 ~PlantController() {
#ifdef DEBUG_MY_CODE
		Serial.println("PlantController DTOR");
#endif

	}

	bool canHandle(Http::HttpServletRequest& req) {
		if(req.httPMethod == HTTP_GET && req.urlTokens.size() > 0 && req.urlTokens[0] == F("plants")){
#ifdef DEBUG_MY_CODE
			Serial.println ("Plant controller can handle the request." );
#endif
			return true;
		}
		return false;
	}

	std::shared_ptr<GardenAcceptable> handle(Http::HttpServletRequest& req, Http::HttpServletResponse&) {
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
