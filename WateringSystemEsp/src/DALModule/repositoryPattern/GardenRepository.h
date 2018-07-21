/*
 * GardenRepository.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_GARDENREPOSITORY_H_
#define DAL_GARDENREPOSITORY_H_

#include <GardenModelContext.h>
#include <Repository.h>

namespace DAL {

class GardenRepository : public Repository<Garden> {
protected:
	IGardenModelContex& _contexModel;
public:
	GardenRepository(IGardenModelContex& gardenModelContext) : _contexModel(gardenModelContext){

	}
	virtual ~GardenRepository() {

	}

	//every id will return the same
	std::shared_ptr<Garden> getById(int ){
		return _contexModel.getGarden();
	}

	std::vector<std::weak_ptr<Garden>> getAll() {
		return std::vector<std::weak_ptr<Garden>>(); //TODO: implement this
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
