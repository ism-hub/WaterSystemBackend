/*
 * ProgramRepository.h
 *
 *  Created on: 11 срхїз 2017
 *      Author: IsM
 */

#ifndef DALMODULE_REPOSITORYPATTERN_PROGRAMREPOSITORY_H_
#define DALMODULE_REPOSITORYPATTERN_PROGRAMREPOSITORY_H_

#include <GardenModelContext.h>
#include <Repository.h>

namespace DAL {

class ProgramRepository : Repository<GardenModel::SimpleProgram>{
protected:
	IGardenModelContex& _contexModel;

public:
	ProgramRepository(IGardenModelContex& contexModel) : _contexModel(contexModel) {

	}
	virtual ~ProgramRepository(){

	}

	std::shared_ptr<GardenModel::SimpleProgram> getById(int id){
		for(auto programm : _contexModel.getGarden()->_programs.getInnerVector()){
			if(programm.get()->id == id)
				return programm.get();
		}
		return nullptr;
	}

	std::vector<std::weak_ptr<GardenModel::SimpleProgram>> getAll() {
		return std::vector<std::weak_ptr<GardenModel::SimpleProgram>>(); //TODO: implement this
	}

	bool add(GardenModel::SimpleProgram&& entity){
		//TODO: implement this.
		return false;
		}

	bool remove(int id){
		//TODO: implement this.
		return false;
	}

};

} /* namespace DAL */

#endif /* DALMODULE_REPOSITORYPATTERN_PROGRAMREPOSITORY_H_ */
