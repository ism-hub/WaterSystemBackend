/*
 * APContex.h
 *
 *  Created on: 27 αιεμι 2018
 *      Author: IsM
 */

#ifndef ACCESSPOINTMODULE_DAL_APCONFCONTEX_H_
#define ACCESSPOINTMODULE_DAL_APCONFCONTEX_H_

#include <FS.h>

#include <JsonSerializationService2.h>
#include <JsonSaveArchive.h>
#include <JsonLoadArchive.h>
#include <DoNothingMappingFile.h>

#include <AccessPointModule/configuration/model/APConfiguration.h>

#include <DALFramework/SPIFFModelContex.h>

namespace apm {

typedef DAL::SerializationService2< mycereal::JsonSaveArchive<mycereal::DoNothingMappingFile>,mycereal::JsonLoadArchive<mycereal::DoNothingMappingFile>> SerializationSerice;
typedef dalfw::SPIFFModelContex<SerializationSerice, APConfiguration> APconfigSPIFFContex;

class APConfContex : public APconfigSPIFFContex {

public:
	APConfContex(std::shared_ptr<SerializationSerice> jsonSerializationService, String savePath) :
		APconfigSPIFFContex(jsonSerializationService, savePath)
	{}
	virtual ~APConfContex() {}



};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_DAL_APCONFCONTEX_H_ */
