/*
 * NtpSettings.h
 *
 *  Created on: 2 баев„ 2018
 *      Author: IsM
 */

#ifndef TIMEMODULE_MODEL_NTPSETTINGS_H_
#define TIMEMODULE_MODEL_NTPSETTINGS_H_

#include <TimeModule/model/ITimeServiceAcceptable.h>
#include <chrono>

namespace tsm {

class NtpSettings {
public:
	String serverIP;
	std::chrono::seconds intervalOnFailure{10};
	std::chrono::minutes intervalOnSuccess{10};
	int timeZone = 2;
	int minutesTimeZone = 0;
	bool daylight = true;
public:
	NtpSettings() {
		serverIP = F("il.pool.ntp.org");
	}
	 ~NtpSettings() {}

//	std::shared_ptr<void> accept(Http::SerializationVisitor<DALModule::DefaultSerializationServerType>& visitor) {
//		return visitor.visit(*this);
//	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const {
		int intervalOnFailure = this->intervalOnFailure.count();
		int intervalOnSuccess = this->intervalOnSuccess.count();
		archive.addProperties(mappingFile, MACRO_NVP(serverIP), MACRO_NVP(intervalOnFailure), MACRO_NVP(intervalOnSuccess), MACRO_NVP(timeZone), MACRO_NVP(minutesTimeZone), MACRO_NVP(daylight));
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
		int intervalOnFailure;
		int intervalOnSuccess;
		archive.loadProperties(mappingFile, MACRO_NVP(serverIP), MACRO_NVP(intervalOnFailure), MACRO_NVP(intervalOnSuccess), MACRO_NVP(timeZone), MACRO_NVP(minutesTimeZone), MACRO_NVP(daylight));
		this->intervalOnFailure = std::chrono::seconds{intervalOnFailure};
		this->intervalOnSuccess = std::chrono::minutes{intervalOnSuccess};

	}
};

} /* namespace sfwkModule */

#endif /* TIMEMODULE_MODEL_NTPSETTINGS_H_ */
