/*
 * PlantRepository.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_PLANTREPOSITORY_H_
#define DAL_PLANTREPOSITORY_H_

#include <GardenModule/DAL/repositoryPattern/GardenModelContext.h>

namespace garden {

class PlantRepository : public Repository<Plant>{
protected:
	IGardenModelContex& _contexModel;

public:
	PlantRepository(IGardenModelContex& contexModel) : _contexModel(contexModel) {
#ifdef DEBUG_MY_CODE
		Serial.println("PlantRepository CTOR");
#endif

	}
	virtual ~PlantRepository() {
#ifdef DEBUG_MY_CODE
		Serial.println("PlantRepository DTOR");
#endif
	}

	std::shared_ptr<Plant> getById(int id){
		return _contexModel.get()->getPlant(id);
	}

	std::vector<std::weak_ptr<Plant>> getAll() {
		std::vector<std::weak_ptr<Plant>> retVec; //TODO: implement this
		for(auto plant : _contexModel.get()->_plants.getInnerVector()) {
			retVec.push_back(plant.get());
		}
		return retVec;
	}

	bool add(Plant&& entity){
		//TODO: implement this.
		return false;
		}

	bool remove(int id){
		//TODO: implement this.
		return false;
	}



};

} /* namespace Http */

#endif /* DAL_PLANTREPOSITORY_H_ */
