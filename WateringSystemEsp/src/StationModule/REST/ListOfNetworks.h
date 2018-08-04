/*
 * ListOfNetworks.h
 *
 *  Created on: 1 баев„ 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_REST_LISTOFNETWORKS_H_
#define STATIONMODULE_REST_LISTOFNETWORKS_H_

#include <vector>

#include <DALFramework/serialization/NameValuePair.h>

#include <StationModule/model/IStationAcceptable.h>
#include <StationModule/model/WiFiNetwork.h>

namespace sm {

//this class exists only so we can send list of networks via the rest API
class ListOfNetworks : public IStationAcceptable {
	const std::vector<WiFiNetwork>& networks;
public:
	ListOfNetworks(const std::vector<WiFiNetwork>& networks) : networks(networks) {}
	virtual ~ListOfNetworks() {}

	std::shared_ptr<void> accept(IStationVisitor& visitor) {
		return visitor.visit(*this);
	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const {
		Serial.println(F("Inside ListOfNetworks save"));
		Serial.print(F("settings heap size: "));Serial.println(ESP.getFreeHeap());
		archive.addProperties(mappingFile, MACRO_NVP(networks));
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
	//	archive.loadProperties(mappingFile, MACRO_NVP(networks));
	}
};

} /* namespace sm */

#endif /* STATIONMODULE_REST_LISTOFNETWORKS_H_ */
