/*
 * APContex.h
 *
 *  Created on: 27 αιεμι 2018
 *      Author: IsM
 */

#ifndef ACCESSPOINTMODULE_DAL_APCONFCONTEX_H_
#define ACCESSPOINTMODULE_DAL_APCONFCONTEX_H_

#include <FS.h>

#include <DALFramework/serializationService/JsonSerializationService2.h>
#include <DALFramework/serialization/JsonSaveArchive.h>
#include <DALFramework/serialization/JsonLoadArchive.h>
#include <DALFramework/serialization/DoNothingMappingFile.h>

#include <AccessPointModule/configuration/model/APConfiguration.h>

#include <DALFramework/SPIFFModelContex.h>

#include <DALFramework/serializationService/DefaultSerializationService.h>

namespace apm {

typedef dalfw::SPIFFModelContex<DALModule::DefaultSerializationService, APConfiguration> APconfigSPIFFContex;

class APConfContex : public APconfigSPIFFContex {

public:
	APConfContex(std::shared_ptr<DALModule::DefaultSerializationService> jsonSerializationService, String savePath) :
		APconfigSPIFFContex(jsonSerializationService, savePath)
	{}
	virtual ~APConfContex() {}



};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_DAL_APCONFCONTEX_H_ */
