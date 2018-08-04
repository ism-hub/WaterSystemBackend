/*
 * ProgramRepository.h
 *
 *  Created on: 11 срхїз 2017
 *      Author: IsM
 */

#ifndef DALMODULE_REPOSITORYPATTERN_PROGRAMREPOSITORY_H_
#define DALMODULE_REPOSITORYPATTERN_PROGRAMREPOSITORY_H_

#include <GardenModule/DAL/repositoryPattern/GardenModelContext.h>
#include <GardenModule/DAL/repositoryPattern/Repository.h>

namespace garden {

class ProgramRepository : Repository<garden::SimpleProgram>{
protected:
	IGardenModelContex& _contexModel;

public:
	ProgramRepository(IGardenModelContex& contexModel) : _contexModel(contexModel) {

	}
	virtual ~ProgramRepository(){

	}

	std::shared_ptr<garden::SimpleProgram> getById(int id){
		for(auto programm : _contexModel.get()->_programs.getInnerVector()){
			if(programm.get()->id == id)
				return programm.get();
		}
		return nullptr;
	}

	std::vector<std::weak_ptr<garden::SimpleProgram>> getAll() {
		return std::vector<std::weak_ptr<garden::SimpleProgram>>(); //TODO: implement this
	}

	bool add(garden::SimpleProgram&& entity){
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
