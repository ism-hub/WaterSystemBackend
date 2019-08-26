/*
 * APStatus.h
 *
 *  Created on: 29 ����� 2018
 *      Author: IsM
 */

#ifndef ACCESSPOINTMODULE_STATUS_MODEL_APSTATUS_H_
#define ACCESSPOINTMODULE_STATUS_MODEL_APSTATUS_H_

#include <AccessPointModule/status/model/IAPStatusAcceptable.h>
#include <ESP8266WiFi.h>
namespace apm {

class APStatus : public IAPStatusAcceptable {
	friend class APService;
protected:
	bool isOn = false;
	String ipAddress = "";
	int numOfConnectedStations = 0;
	String macAddress = "";
public:
	APStatus() {init();}
	virtual ~APStatus() {}

	//reset to default values
	void init() {
		isOn = false;
		ipAddress = "";
		numOfConnectedStations = 0;
		macAddress = "";
	}

	bool getIsOn() const{
		return isOn;
	}
	String getIpAddress() const{
		return ipAddress;
	}
	int getNumOfConnectedStations() const{
		return numOfConnectedStations;
	}
	String getMACAddress() const{
		return macAddress;
	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const {
		archive.addProperties(mappingFile, MACRO_NVP(isOn), MACRO_NVP(ipAddress), MACRO_NVP(numOfConnectedStations), MACRO_NVP(macAddress));
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
	}

	virtual std::shared_ptr<void> accept(Http::SerializationVisitor<DALModule::DefaultSerializationService, String>& visitor) {
		return visitor.visit(*this);
	}
};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_STATUS_MODEL_APSTATUS_H_ */
