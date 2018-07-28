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
#include <Garden.h>

#include <FS.h>

#include <JsonSerializationService2.h>
#include <JsonSaveArchive.h>
#include <JsonLoadArchive.h>
#include <FlashMappingFile.h>
#include <IGardenModelContex.h>

#include <DALFrameWork/SPIFFModelContex.h>

using namespace GardenModel;

namespace DAL {
typedef DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::FlashMappingFile>,mycereal::JsonLoadArchive<DAL::FlashMappingFile>> FlashSerializationSerice;
typedef dalfw::SPIFFModelContex<FlashSerializationSerice, Garden> GardenSPIFFContex;

class GardenModelContext : public GardenSPIFFContex, public IGardenModelContex {
public:

	GardenModelContext(std::shared_ptr<FlashSerializationSerice> jsonSerializationService, String savePath = "garden.json") :
		GardenSPIFFContex(jsonSerializationService, savePath)
	{}

	virtual ~GardenModelContext(){}

	std::shared_ptr<GardenModel::Garden> get() override {
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
