/*
 * DBContext.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_GARDENMODELCONTEXT_H_
#define DAL_GARDENMODELCONTEXT_H_

#include <Garden.h>
#include <JsonGardenVisitor.h>
//#include <GardenRepository.h>
//#include <PlantRepository.h>
//#include <SprinklerRepository.h>

class GardenRepository;
class PlantRepository;
class SprinklerRepository;

using namespace GardenModel;

namespace DAL {

class GardenModelContext {
public:
	Garden* 				_garden = NULL;

	GardenRepository* 		_gardenRepository;
	PlantRepository* 		_plantRepository;
	SprinklerRepository* 	_sprinklerRepository;

	JsonGardenVisitor jsonSerializer;

	GardenModelContext(){
		loadGardenIntoMemory();
		_gardenRepository = new GardenRepository(this);
		_plantRepository = new PlantRepository(this);
		_sprinklerRepository = new SprinklerRepository(this);

	}

	virtual ~GardenModelContext(){

	}

	bool saveChanges(){

		return false;
	}

private:
	void loadGardenIntoMemory() {

	}
};

} /* namespace Http */

#endif /* DAL_GARDENMODELCONTEXT_H_ */
