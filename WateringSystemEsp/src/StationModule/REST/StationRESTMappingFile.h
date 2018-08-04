/*
 * StationRESTMappingFile.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_REST_STATIONRESTMAPPINGFILE_H_
#define STATIONMODULE_REST_STATIONRESTMAPPINGFILE_H_

#include <functional>
#include <vector>
#include <algorithm>
#include <DALFramework/serialization/JsonContex.h>

#include <StationModule/DAL/StationContex.h>
#include <StationModule/model/Station.h>
#include <StationModule/model/WiFiNetwork.h>

#include <stuff/stuff.h>

namespace sm {

class StationRESTMappingFile {
	std::shared_ptr<StationContex> stationContex = nullptr;
public:
	StationRESTMappingFile(std::shared_ptr<StationContex> stationContex) : stationContex(stationContex) {
		Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~ StationRESTMappingFile CTOR"));
		if(stationContex == nullptr)
			Serial.println(F("_______________ERROR___________ stationContex == null"));
	}
	 ~StationRESTMappingFile() {
		Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~ StationRESTMappingFile DTOR"));
	}

	template<typename ... Args>
	void Model2Json(Args& ...) {}

	template<typename ArchiveType>
	void Model2Json(const Station& station, ArchiveType& archive)  {
		Serial.println(F("Inside Model2Json(const Station&  in the REST mapping file "));
		std::vector<stf::Link> links;
		links.push_back(stf::Link(F("savedNetworks"),F("station/savedNetworks")));
		links.push_back(stf::Link(F("connectionStatus"),F("station/connectionStatus")));
		links.push_back(stf::Link(F("nearbyNetworks"),F("station/nearbyNetworks")));
		if(station.getConnectionStatus().isConnected)
			links.push_back(stf::Link(F("disconnect"),F("station/disconnect")));
		archive.addProperties(*this, MACRO_NVP(links));
		DAL::JsonContex& context = archive.getCurrentContex();
		context.listOfNotAllowedKeys.push_back(F("savedNetworks"));
		context.listOfNotAllowedKeys.push_back(F("connectionStatus"));
	}

	template<typename ArchiveType>
	void Model2Json(const WiFiNetwork& , ArchiveType& )  {
		delay(0);
		if(stationContex == nullptr)
			Serial.println(F("__________ERROR_______ void Model2Json(const WiFiNetwork& wifiNet, ArchiveTy stationContex == nullptr"));
	/*	std::vector<stf::Link> links;
		if(stationContex->get()->isInSavedNetworks(wifiNet))
			links.push_back(stf::Link(F("forget"),"station/savedNetworks/" + wifiNet.SSID + "/forget"));
		if(stationContex->get()->isNearbyNetwork(wifiNet) && wifiNet != stationContex->get()->getConnectionStatus().network)
			links.push_back(stf::Link(F("connect"),"station/nearbyNetworks/" + wifiNet.SSID + "/connect"));
		if(stationContex->get()->getConnectionStatus().isConnected && wifiNet == stationContex->get()->getConnectionStatus().network) {
			links.push_back(stf::Link(F("disconnect"),"station/nearbyNetworks/" + wifiNet.SSID + "/disconnect"));
			links.push_back(stf::Link(F("save"),"station/nearbyNetworks/" + wifiNet.SSID + "/save"));
		}
		if(links.size() != 0)
			archive.addProperties(*this, MACRO_NVP(links));
		else
			Serial.println(F("links.size() == 0"));*/
	}

	template<typename ... Args>
	void Json2Model(Args& ...) {}

	template<typename T, typename A>
	std::function<T* (std::vector<T, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<T, A>&, DAL::JsonContex&)
	{return std::function<T* (std::vector<T, A>&, DAL::JsonContex&)>();}
};

} /* namespace sm */

#endif /* STATIONMODULE_DAL_STATIONRESTMAPPINGFILE_H_ */
