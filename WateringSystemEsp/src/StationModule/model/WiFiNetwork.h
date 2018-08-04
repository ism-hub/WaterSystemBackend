/*
 * WiFiNetwork.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_MODEL_WIFINETWORK_H_
#define STATIONMODULE_MODEL_WIFINETWORK_H_

#include <WString.h>
#include <include/wl_definitions.h>
namespace sm {

class WiFiNetwork : public IStationAcceptable {
public:
	String SSID = "";
	int RSSI = 0;
	String BSSID = "";
	int channel = 0;
	wl_enc_type encryptionType = wl_enc_type::ENC_TYPE_NONE;
	String password = "";
	bool autoConnect = false;

	WiFiNetwork(String SSID, int RSSI, String BSSID, int channel, wl_enc_type encryptionType, String password = "", bool autoConnect = false) {
		this->SSID = SSID;
		this->RSSI = RSSI;
		this->BSSID = BSSID;
		this->channel = channel;
		this->encryptionType = encryptionType;
		this->password = password;
		this->autoConnect = autoConnect;
	}
	WiFiNetwork() {}
	virtual ~WiFiNetwork() {}

	bool operator==(const WiFiNetwork& rhs) const{
		return (SSID == rhs.SSID && encryptionType == rhs.encryptionType && BSSID == rhs.BSSID);
	}

	bool operator<(const WiFiNetwork& rhs) const{
		return (RSSI < rhs.RSSI);
	}

	bool operator>(const WiFiNetwork& rhs) const{
		return (RSSI > rhs.RSSI);
	}

	bool isPasswordSaved() {
		return !(password == "" && encryptionType == wl_enc_type::ENC_TYPE_NONE);
	}

	std::shared_ptr<void> accept(IStationVisitor& visitor) {
		return visitor.visit(*this);
	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const {
		archive.addProperties(mappingFile, MACRO_NVP(SSID), MACRO_NVP(RSSI), MACRO_NVP(BSSID), MACRO_NVP(channel));
		int encryptionType = static_cast<int>(this->encryptionType);
		archive.addProperties(mappingFile, MACRO_NVP(encryptionType), MACRO_NVP(password), MACRO_NVP(autoConnect));
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
		archive.loadProperties(mappingFile, MACRO_NVP(SSID), MACRO_NVP(RSSI), MACRO_NVP(BSSID), MACRO_NVP(channel));
		int encryptionType;
		archive.loadProperties(mappingFile, MACRO_NVP(encryptionType), MACRO_NVP(password), MACRO_NVP(autoConnect));
		this->encryptionType = static_cast<wl_enc_type>(encryptionType);
	}
};

} /* namespace sm */

#endif /* STATIONMODULE_MODEL_WIFINETWORK_H_ */
