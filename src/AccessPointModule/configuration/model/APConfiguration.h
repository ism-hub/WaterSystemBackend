/*
 * APConfiguration.h
 *
 *  Created on: 27 ����� 2018
 *      Author: IsM
 */
#ifndef ACCESSPOINTMODULE_MODEL_APCONFIGURATION_H_
#define ACCESSPOINTMODULE_MODEL_APCONFIGURATION_H_


#include <WString.h>
#include <DALFramework/serializationService/JsonSerializationService2.h>

#include <HttpFramework.hpp>
#include <AccessPointModule/configuration/model/IAPConfAcceptable.h>

namespace apm {

class APConfiguration : public IAPConfAcceptable {
	template <typename E>
	constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
	    return static_cast<typename std::underlying_type<E>::type>(e);
	}
public:
	enum class WhenOn {WHEN_WIFI_DISCONNECTED = 0x01, WHEN_NO_ONE_CONNECTED = 0x02, WHEN_WIFI_DISCONNECTED_AND_WHEN_NO_ONE_CONNECTED = 0x03, ALWAYS = 0xFF };
	WhenOn whenOn = WhenOn::WHEN_WIFI_DISCONNECTED_AND_WHEN_NO_ONE_CONNECTED;//when to provide access-point
	String ssid;
	String password;
	String apConfigRestAPIPath;
	String localIP;
	String gateway;
	String subnet;
public:
	APConfiguration() {
		ssid = F("ESP8266123");
		password = F("esp8266123");
		apConfigRestAPIPath = F("apconfig");
		localIP = F("192.168.69.1");
		gateway = F("192.168.69.255");
		subnet = F("255.255.255.0");
	}
	virtual ~APConfiguration() {}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const {
		archive.addProperties(mappingFile, MACRO_NVP(ssid), MACRO_NVP(password), MACRO_NVP(apConfigRestAPIPath));
		archive.addProperties(mappingFile, MACRO_NVP(localIP), MACRO_NVP(gateway), MACRO_NVP(subnet));
		int whenOn = to_underlying(this->whenOn);
		archive.addProperties(mappingFile, MACRO_NVP(whenOn));
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
		archive.loadProperties(mappingFile, MACRO_NVP(ssid), MACRO_NVP(password), MACRO_NVP(apConfigRestAPIPath));
		archive.loadProperties(mappingFile, MACRO_NVP(localIP), MACRO_NVP(gateway), MACRO_NVP(subnet));
		int whenOn = to_underlying(this->whenOn);
		archive.loadProperties(mappingFile, MACRO_NVP(whenOn));
		this->whenOn = static_cast<WhenOn>(whenOn);

	}

	virtual std::shared_ptr<void> accept(Http::SerializationVisitor<DALModule::DefaultSerializationService, String>& visitor) {
		return visitor.visit(*this);
	}

};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_MODEL_APCONFIGURATION_H_ */
