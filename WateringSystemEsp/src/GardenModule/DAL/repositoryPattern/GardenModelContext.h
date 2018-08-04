/*
 * DBContext.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_GARDENMODELCONTEXT_H_
#define DAL_GARDENMODELCONTEXT_H_

//#include <DALModule/serialization/cereal2.h>
//#include <DALModule/serialization/json2.h>
#include <DALFramework/SPIFFModelContex.h>
#include <GardenModule/model/Garden.h>

#include <FS.h>

#include <GardenModule/DAL/GardenFlashSerializationService.h>

#include <GardenModule/DAL/repositoryPattern/IGardenModelContex.h>


namespace garden {

typedef dalfw::SPIFFModelContex<garden::GardenFlashSerializationService, Garden> GardenSPIFFContex;

class GardenModelContext : public GardenSPIFFContex, public IGardenModelContex {
public:

	GardenModelContext(std::shared_ptr<garden::GardenFlashSerializationService> gardenFlashSerializationService, String savePath = "garden.json") :
		GardenSPIFFContex(gardenFlashSerializationService, savePath)
	{}

	virtual ~GardenModelContext(){}

	std::shared_ptr<garden::Garden> get() override {
		return GardenSPIFFContex::get();
	}

	bool init() override {
		return GardenSPIFFContex::init();
	}

	bool save() override {
		return GardenSPIFFContex::save();
	}


};

} /* namespace Http */

#endif /* DAL_GARDENMODELCONTEXT_H_ */
