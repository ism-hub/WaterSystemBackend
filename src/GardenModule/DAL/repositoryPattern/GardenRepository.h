/*
 * GardenRepository.h
 *
 *  Created on: 26 ����� 2017
 *      Author: IsM
 */

#ifndef DAL_GARDENREPOSITORY_H_
#define DAL_GARDENREPOSITORY_H_

#include <GardenModule/DAL/repositoryPattern/GardenModelContext.h>
#include <GardenModule/DAL/repositoryPattern/Repository.h>

namespace garden {

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
		return _contexModel.get();
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
