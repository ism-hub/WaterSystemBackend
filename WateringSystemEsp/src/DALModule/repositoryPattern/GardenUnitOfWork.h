/*
 * GardenUnitOfWork.h
 *
 *  Created on: 27 ����� 2017
 *      Author: IsM
 */

#ifndef DAL_GARDENUNITOFWORK_H_
#define DAL_GARDENUNITOFWORK_H_

#include <DALModule/repositoryPattern/GardenRepository.h>
#include <DALModule/repositoryPattern/PlantRepository.h>
#include <DALModule/repositoryPattern/SprinklerRepository.h>
#include <DALModule/repositoryPattern/ProgramRepository.h>
#include <memory>

namespace DAL {

class GardenUnitOfWork {
private:
	GardenRepository 		_gardenRepository;
	PlantRepository 		_plantRepository;
	SprinklerRepository 	_sprinklerRepository;
	ProgramRepository 		_programRepository;
public:
	std::shared_ptr<GardenModelContext> 	_gardenContext;

public:
	GardenUnitOfWork(std::shared_ptr<GardenModelContext> gardenContext) :// check what happens object wise, the objects i initiated here getting deleted after the CTOR end or after the object end
		_gardenRepository(GardenRepository(*gardenContext)),
		_plantRepository(PlantRepository(*gardenContext)),
		_sprinklerRepository( SprinklerRepository(*gardenContext)),
		_programRepository( ProgramRepository(*gardenContext)),
		_gardenContext(gardenContext)
		{
	//	_gardenContext->init();
	}

	virtual ~GardenUnitOfWork(){

	}

	GardenRepository& Garden(){
		return _gardenRepository;
	}

	PlantRepository& Plants(){
		return _plantRepository;
	}

	SprinklerRepository& Sprinklers(){
		return _sprinklerRepository;
	}

	ProgramRepository& Programs(){
		return _programRepository;
	}

	bool complete(){
		return _gardenContext->saveChanges();
	}

};

} /* namespace DAL */

#endif /* DAL_GARDENUNITOFWORK_H_ */