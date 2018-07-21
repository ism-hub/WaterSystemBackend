/*
 * SprinklerRepository.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_SPRINKLERREPOSITORY_H_
#define DAL_SPRINKLERREPOSITORY_H_

#include <GardenModelContext.h>

namespace DAL {

class SprinklerRepository : Repository<Sprinkler>{
protected:
	IGardenModelContex& _contexModel;
public:
	SprinklerRepository(IGardenModelContex& contexModel) : _contexModel(contexModel) {

	}
	virtual ~SprinklerRepository() {

	}

	std::shared_ptr<Sprinkler> getById(int id){
		return {};//TODO: implement this.
	}

	std::vector<std::weak_ptr<Sprinkler>> getAll() {
		std::vector<std::weak_ptr<Sprinkler>> retVec; //TODO: implement this
		for(auto& sprinkler : _contexModel.getGarden()->_sprinklers.getInnerVector()) {
			retVec.push_back(sprinkler.get());
		}
		return retVec;
		//return std::vector<std::weak_ptr<Sprinkler>>(); //TODO: implement this
	}

	bool add(Sprinkler&& entity){
		//TODO: implement this.
		return false;
	}

	bool remove(int id){
		//TODO: implement this.
		return false;
	}

};

} /* namespace Http */

#endif /* DAL_SPRINKLERREPOSITORY_H_ */
