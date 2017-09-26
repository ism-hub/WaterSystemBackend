/*
 * SprinklerController.h
 *
 *  Created on: 13 ����� 2017
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
#include <httpModule/dispatcher/Controller.h>
#include <httpModule/model/HttpServletRequest.h>
#include <httpModule/model/HttpServletResponse.h>


using namespace std;
using namespace GardenModel;

namespace Http {

class PlantController : public Controller{

protected:
	std::shared_ptr<DAL::GardenUnitOfWork> _unitOfWork;

public:

	PlantController(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork): _unitOfWork(unitOfWork) {
	}

	virtual ~PlantController() {
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ PlantController DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	bool canHandle(HttpServletRequest& req) {
		Serial.println ("we are in the canHandle() of the PlantController" );
		if(req.httPMethod == HTTP_GET && req.urlTokens[0] == "plants"){
			Serial.println ("	- can handle returned true" );
			return true;
		}
		return false;
	}

	GardenAcceptable* handle(HttpServletRequest& req, HttpServletResponse&) {
		Plant* plant = NULL;
		if(canHandle(req) && req.urlTokens.size() == 2){///GET PLANTS_ID
			Serial.println ("we handling the message" );
			//get parameters
			int id=atoi(req.urlTokens[1].c_str());
			//call the correct function
			plant = _unitOfWork->Plants().getById(id).lock().get();
		}
		return plant;
	}

	/*list<Sprinkler> getSprinklers() {

	}*/
};

} /* namespace Http */

#endif /* HTTPMODULE_CONTROLLERS_PLANTCONTROLLER_H_ */
