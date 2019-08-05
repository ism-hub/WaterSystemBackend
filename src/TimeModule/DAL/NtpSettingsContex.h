/*
 * NtpSettingsContex.h
 *
 *  Created on: 2 баев„ 2018
 *      Author: IsM
 */

#ifndef TIMEMODULE_DAL_NTPSETTINGSCONTEX_H_
#define TIMEMODULE_DAL_NTPSETTINGSCONTEX_H_

#include <DALFramework/serializationService/JsonSerializationService2.h>

#include <DALFramework/serialization/DoNothingMappingFile.h>
#include <DALFramework/serialization/JsonSaveArchive.h>
#include <DALFramework/serialization/JsonLoadArchive.h>
#include <DALFramework/SPIFFModelContex.h>

#include <TimeModule/model/NtpSettings.h>

#include <DALModule/DALModule.h>

namespace tsm {

typedef dalfw::SPIFFModelContex<DALModule::DefaultSerializationService, NtpSettings> NTPSettingsPIFFContex;
class NtpSettingsContex : public NTPSettingsPIFFContex {
public:
	NtpSettingsContex(std::shared_ptr<DALModule::DefaultSerializationService> jsonSerializationService, String savePath) :
		NTPSettingsPIFFContex(jsonSerializationService, savePath) {
	//	Serial.println("---------------- NtpSettingsContex CTOR");
	}
	 ~NtpSettingsContex() {}
};

} /* namespace tsm */

#endif /* TIMEMODULE_DAL_NTPSETTINGSCONTEX_H_ */
