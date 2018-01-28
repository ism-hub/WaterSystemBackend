/*
 * PlantRepository.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_PLANTREPOSITORY_H_
#define DAL_PLANTREPOSITORY_H_

#include <DALModule/repositoryPattern/GardenModelContext.h>

namespace DAL {

class PlantRepository : public Repository<Plant>{
protected:
	GardenModelContext& _contexModel;

public:
	PlantRepository(GardenModelContext& contexModel) : _contexModel(contexModel) {
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
		return _contexModel.getGarden()->getPlant(id);
	}

	std::vector<std::weak_ptr<Plant>> getAll() {
		return std::vector<std::weak_ptr<Plant>>(); //TODO: implement this
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
