/*
 * ConnectionStatus.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_MODEL_CONNECTIONSTATUS_H_
#define STATIONMODULE_MODEL_CONNECTIONSTATUS_H_

#include <StationModule/model/IStationVisitor.h>
#include <StationModule/model/IStationAcceptable.h>

namespace sm {

class ConnectionStatus : public IStationAcceptable {
public:
	sm::WiFiNetwork network;
	bool isConnected = false;
	String localIp = "";
public:
	ConnectionStatus() {
		init();
	}
	virtual ~ConnectionStatus() {}

	void init() {
		isConnected = false;
		localIp = "";
		network = sm::WiFiNetwork();
	}

	std::shared_ptr<void> accept(IStationVisitor& visitor) {
		return visitor.visit(*this);
	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const {
		archive.addProperties(mappingFile, MACRO_NVP(network), MACRO_NVP(isConnected), MACRO_NVP(localIp));
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
		archive.loadProperties(mappingFile, MACRO_NVP(network), MACRO_NVP(isConnected), MACRO_NVP(localIp));
	}

};

} /* namespace sm */

#endif /* STATIONMODULE_MODEL_CONNECTIONSTATUS_H_ */
