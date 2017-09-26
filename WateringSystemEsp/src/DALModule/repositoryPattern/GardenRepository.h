/*
 * GardenRepository.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_GARDENREPOSITORY_H_
#define DAL_GARDENREPOSITORY_H_

#include <DALModule/repositoryPattern/GardenModelContext.h>
#include <DALModule/repositoryPattern/Repository.h>

namespace DAL {

class GardenRepository : public Repository<Garden> {
protected:
	GardenModelContext& _contexModel;
public:
	GardenRepository(GardenModelContext& gardenModelContext) : _contexModel(gardenModelContext){

	}
	virtual ~GardenRepository() {

	}

	weak_ptr<Garden> getById(int id){
		return {};//TODO: implement this.
	}

	std::vector<weak_ptr<Garden>> getAll() {
		return std::vector<weak_ptr<Garden>>(); //TODO: implement this
	}

	bool add(Garden&& entity){
		//TODO: implement this.
		return false;
		}

	bool remove(int id){
		//TODO: implement this.
		return false;
	}


};

} /* namespace Http */

#endif /* DAL_GARDENREPOSITORY_H_ */
