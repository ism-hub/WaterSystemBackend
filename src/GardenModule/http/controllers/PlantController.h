/*
 * SprinklerController.h
 *
 *  Created on: 13 ����� 2017
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
#include <HttpFramework.hpp>




namespace garden {

class PlantController : public Http::IController<garden::GardenAcceptable, String> {

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

	bool canHandle(Http::HttpServletRequest<String>& req) {
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() > 0 && req.urlTokens[0] == String(F("plants"))){
#ifdef DEBUG_MY_CODE
			Serial.println ("Plant controller can handle the request." );
#endif
			return true;
		}
		return false;
	}

	std::shared_ptr<GardenAcceptable> handle(Http::HttpServletRequest<String>& req, Http::HttpServletResponse<String>&) {
		std::shared_ptr<Plant> plant = nullptr;
		if(canHandle(req) && req.urlTokens.size() == 2){///GET PLANTS_ID
			//get parameters
			int id=req.urlTokens[1].stoi();
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
