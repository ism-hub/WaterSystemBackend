/*
 * APConfiguration.h
 *
 *  Created on: 27 αιεμι 2018
 *      Author: IsM
 */
#ifndef ACCESSPOINTMODULE_MODEL_APCONFIGURATION_H_
#define ACCESSPOINTMODULE_MODEL_APCONFIGURATION_H_


#include <WString.h>
#include <JsonSerializationService2.h>

#include <httpModule/interceptors/SerializationVisitor.h>
#include <AccessPointModule/model/IAPConfAcceptable.h>

namespace apm {

class APConfiguration : public IAPConfAcceptable {

public:
	bool provisionMode = false;
	String ssid = "ESP8266123";
	String password = "esp8266123";
	String apConfigRestAPIPath = "apconfig";
public:
	APConfiguration() {}
	virtual ~APConfiguration() {}

	template <class Archive>
	void save(Archive& archive) const {
		archive.addProperties(MACRO_NVP(provisionMode), MACRO_NVP(ssid), MACRO_NVP(password), MACRO_NVP(apConfigRestAPIPath));
	}

	template<class Archive>
	void load(Archive& archive) {
		archive.loadProperties(MACRO_NVP(provisionMode), MACRO_NVP(ssid), MACRO_NVP(password), MACRO_NVP(apConfigRestAPIPath));
	}

	virtual std::shared_ptr<void> accept(Http::SerializationVisitor<serializationServiceType>& visitor) {
		return visitor.visit(*this);
	}

};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_MODEL_APCONFIGURATION_H_ */
