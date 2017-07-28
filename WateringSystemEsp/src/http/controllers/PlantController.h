/*
 * SprinklerController.h
 *
 *  Created on: 13 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_CONTROLLERS_PLANTCONTROLLER_H_
#define HTTP_CONTROLLERS_PLANTCONTROLLER_H_

#include <list>
#include <Plant.h>
#include <HttpServletRequest.h>
#include <HttpServletResponse.h>
#include <stdlib_noniso.h>//atoi
#include <controllers/Controller.h>
#include <GardenAcceptable.h>
#include <Garden.h>

using namespace std;
using namespace GardenModel;

namespace Http {

class PlantController : public Controller{
public:

	Garden& _garden;

	PlantController(Garden& garden): _garden(garden) {
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

	GardenAcceptable* handle(HttpServletRequest& req, HttpServletResponse& res) {
		Plant* plant = NULL;
		if(canHandle(req) && req.urlTokens.size() == 2){///GET PLANTS_ID
			Serial.println ("we handling the message" );
			//get parameters
			int id=atoi(req.urlTokens[1].c_str());
			//call the correct function
			plant = getPlant(id).lock().get();
		}
		return plant;
	}

	/*list<Sprinkler> getSprinklers() {

	}*/

	///GET SPRINKLERS_ID
	weak_ptr<Plant> getPlant(int id) {
		Serial.println ("we called getPlant in the PlantController" );
		return _garden.getPlant(id);
	}
};

} /* namespace Http */

#endif /* HTTP_CONTROLLERS_PLANTCONTROLLER_H_ */
