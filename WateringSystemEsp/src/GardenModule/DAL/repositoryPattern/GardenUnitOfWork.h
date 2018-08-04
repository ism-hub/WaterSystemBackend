/*
 * GardenUnitOfWork.h
 *
 *  Created on: 27 баев„ 2017
 *      Author: IsM
 */

#ifndef DAL_GARDENUNITOFWORK_H_
#define DAL_GARDENUNITOFWORK_H_

#include <GardenModule/DAL/repositoryPattern/GardenRepository.h>
#include <GardenModule/DAL/repositoryPattern/PlantRepository.h>
#include <GardenModule/DAL/repositoryPattern/SprinklerRepository.h>
#include <GardenModule/DAL/repositoryPattern/ProgramRepository.h>
#include <memory>
#include <GardenModule/DAL/repositoryPattern/IGardenModelContex.h>

namespace garden {

class GardenUnitOfWork {
private:
	GardenRepository 		_gardenRepository;
	PlantRepository 		_plantRepository;
	SprinklerRepository 	_sprinklerRepository;
	ProgramRepository 		_programRepository;
public:
	std::shared_ptr<IGardenModelContex> 	_gardenContext;

public:
	GardenUnitOfWork(std::shared_ptr<IGardenModelContex> gardenContext) :// check what happens object wise, the objects i initiated here getting deleted after the CTOR end or after the object end
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
		return _gardenContext->save();
	}

};

} /* namespace DAL */

#endif /* DAL_GARDENUNITOFWORK_H_ */
