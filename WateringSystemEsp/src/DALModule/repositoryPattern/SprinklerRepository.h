/*
 * SprinklerRepository.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_SPRINKLERREPOSITORY_H_
#define DAL_SPRINKLERREPOSITORY_H_

#include <DALModule/repositoryPattern/GardenModelContext.h>

namespace DAL {

class SprinklerRepository : Repository<Sprinkler>{
protected:
	GardenModelContext& _contexModel;
public:
	SprinklerRepository(GardenModelContext& contexModel) : _contexModel(contexModel) {

	}
	virtual ~SprinklerRepository() {

	}

	weak_ptr<Sprinkler> getById(int id){
			return {};//TODO: implement this.
		}

		std::vector<weak_ptr<Sprinkler>> getAll() {
			return std::vector<weak_ptr<Sprinkler>>(); //TODO: implement this
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
