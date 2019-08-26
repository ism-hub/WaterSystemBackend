/*
 * StationController.h
 *
 *  Created on: 31 ����� 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_REST_STATIONCONTROLLER_H_
#define STATIONMODULE_REST_STATIONCONTROLLER_H_

#include <HttpFramework.hpp>
#include <memory>

#include <StationModule/model/IStationAcceptable.h>
#include <StationModule/model/Station.h>
#include <StationModule/service/StationIService.h>
#include <StationModule/REST/StationRESTMappingFile.h>
#include <StationModule/REST/ListOfNetworks.h>

#include <StationModule/REST/defs.h>

#include <StationModule/REST/StationRESTSerializationService.h>

namespace sm {


class StationController : public Http::IController<IStationAcceptable, String> {
	std::shared_ptr<StationIService> stationService;
	std::shared_ptr<StationRESTSerializationService> _restSerializationService;
public:
	StationController(std::shared_ptr<StationIService> stationService, std::shared_ptr<StationRESTSerializationService> _restSerializationService) :
		stationService(stationService), _restSerializationService(_restSerializationService) {
	//	Serial.println("---------------------------- StationController CTOR");
	}
	 ~StationController() {
//		Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~ StationController DTOR");
	}

	bool canHandle(Http::HttpServletRequest<String>& req) {
		if(req.urlTokens.size() < 1 || req.urlTokens[0] != String(F("station")))
			return false;
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() == 1) {
			return true;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() == 2 ) {
			if(req.urlTokens[1] == String(F("savedNetworks")) || req.urlTokens[1] == String(F("connectionStatus")) || req.urlTokens[1] == String(F("nearbyNetworks")))
				return true;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 2 ) {
			if(req.urlTokens[1] == String(F("disconnect")))
				return true;
			if(req.urlTokens[1] == String(F("connect")))
				return true;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 4) {
			if(req.urlTokens[1] == String(F("nearbyNetworks")) && (req.urlTokens[3] == String(F("connect")) || req.urlTokens[3] == String(F("disconnect")) || req.urlTokens[3] == String(F("save"))))
				return true;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 4) {
			if(req.urlTokens[1] == String(F("savedNetworks")) && req.urlTokens[3] == String(F("forget")))
				return true;
		}
		return false;
	}

	std::shared_ptr<IStationAcceptable> handle(Http::HttpServletRequest<String>& req, Http::HttpServletResponse<String>& ) {
		std::shared_ptr<Station> station = stationService->getStation();
	//	if(station == nullptr)
	//		Serial.println("_________ERROR_______ stationService->getStation() == nullptr in StationController");
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() == 1){
	//		Serial.println("Inside handle of '/station'");
			return station;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() == 2) {
			if(req.urlTokens[1] == String(F("savedNetworks")))
				return std::make_shared<ListOfNetworks>(station->getSavedNetworks());
			if(req.urlTokens[1] == String(F("connectionStatus")))
				return std::make_shared<ConnectionStatus>(station->getConnectionStatus());
			if(req.urlTokens[1] == String(F("nearbyNetworks"))){
				Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
				Serial.println(F("Handling nearbyNetworks GET request"));
				std::vector<WiFiNetwork>& networks = station->getNearbyNetworks();
		//		Serial.println("B4    auto ret =  std::make_shared<ListOfNetworks>(networks);");
				Serial.printf("b4 std::make_shared<ListOfNetworks>(networks) settings heap size: %u\n", ESP.getFreeHeap());
				//taking only the strongest 7
				std::shared_ptr<ListOfNetworks> ret =  std::make_shared<ListOfNetworks>(networks);
				Serial.printf("after settings heap size: %u\n", ESP.getFreeHeap());
		//		Serial.println("Done handling nearbyNetworks GET request in the controller");
				return ret;
			}
		}

		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 2){
			if(req.urlTokens[1] == String(F("disconnect"))){
				station->disconnect();
				return  std::make_shared<ConnectionStatus>(station->getConnectionStatus());
			}
			if(req.urlTokens[1] == String(F("connect"))){
				connectJson(req.requestBody);
				return  std::make_shared<ConnectionStatus>(station->getConnectionStatus());
			}
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 4) {
			if(req.urlTokens[1] == String(F("nearbyNetworks"))){
				if( req.urlTokens[3] == String(F("connect"))){
					connectJson(req.requestBody);
					return  std::make_shared<ConnectionStatus>(station->getConnectionStatus());
				}
				if(req.urlTokens[3] == String(F("disconnect"))){
					station->disconnect();
					return  std::make_shared<ConnectionStatus>(station->getConnectionStatus());
				}
				if(req.urlTokens[3] == String(F("save"))) {//saving the network we currently connected to
					station->saveNetwork(station->getConnectionStatus().network);
					return std::make_shared<ListOfNetworks>(station->getSavedNetworks());
				}
			}
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 4) {
			if(req.urlTokens[1] == String(F("savedNetworks")) && req.urlTokens[3] == String(F("forget"))){
				station->forgetNetwork((String)req.urlTokens[2]);
				return std::make_shared<ListOfNetworks>(station->getSavedNetworks());
			}
		}
		Serial.println(F("__ERROR___: (inside StationController::handle)"));
		return nullptr;
	}

protected:
	void connectJson(const String& json) {
		WiFiNetwork network;
		_restSerializationService->Json2Model(network, json);
		stationService->getStation()->connect(network, true, [=](){stationService->saveStation();});
	}
};

} /* namespace sm */

#endif /* STATIONMODULE_REST_STATIONCONTROLLER_H_ */
