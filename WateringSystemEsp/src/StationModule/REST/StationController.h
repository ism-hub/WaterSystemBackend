/*
 * StationController.h
 *
 *  Created on: 31 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_REST_STATIONCONTROLLER_H_
#define STATIONMODULE_REST_STATIONCONTROLLER_H_

#include <HttpFramework/dispatcher/IController.h>
#include <HttpFramework/model/HttpServletRequest.h>
#include <HttpFramework/model/HttpServletResponse.h>
#include <memory>

#include <StationModule/model/IStationAcceptable.h>
#include <StationModule/model/Station.h>
#include <StationModule/service/StationIService.h>
#include <StationModule/REST/StationRESTMappingFile.h>
#include <StationModule/REST/ListOfNetworks.h>

#include <StationModule/REST/defs.h>

#include <StationModule/REST/StationRESTSerializationService.h>

namespace sm {


class StationController : public Http::IController<IStationAcceptable> {
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

	bool canHandle(Http::HttpServletRequest& req) {
		if(req.urlTokens.size() < 1 || req.urlTokens[0] != F("station"))
			return false;
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() == 1) {
			return true;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() == 2 ) {
			if(req.urlTokens[1] == F("savedNetworks") || req.urlTokens[1] == F("connectionStatus") || req.urlTokens[1] == F("nearbyNetworks"))
				return true;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 2 ) {
			if(req.urlTokens[1] == F("disconnect"))
				return true;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 4) {
			if(req.urlTokens[1] == F("nearbyNetworks") && (req.urlTokens[3] == F("connect") || req.urlTokens[3] == F("disconnect") || req.urlTokens[3] == F("save")))
				return true;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 4) {
			if(req.urlTokens[1] == F("savedNetworks") && req.urlTokens[3] == F("forget"))
				return true;
		}
		return false;
	}

	std::shared_ptr<IStationAcceptable> handle(Http::HttpServletRequest& req, Http::HttpServletResponse& ) {
		std::shared_ptr<Station> station = stationService->getStation();
	//	if(station == nullptr)
	//		Serial.println("_________ERROR_______ stationService->getStation() == nullptr in StationController");
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() == 1){
	//		Serial.println("Inside handle of '/station'");
			return station;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() == 2) {
			if(req.urlTokens[1] == F("savedNetworks"))
				return std::make_shared<ListOfNetworks>(station->getSavedNetworks());
			if(req.urlTokens[1] == F("connectionStatus"))
				return std::make_shared<ConnectionStatus>(station->getConnectionStatus());
			if(req.urlTokens[1] == F("nearbyNetworks")){
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
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 2)
			if(req.urlTokens[1] == F("disconnect")){
				station->disconnect();
				return  std::make_shared<ConnectionStatus>(station->getConnectionStatus());
			}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 4) {
			if(req.urlTokens[1] == F("nearbyNetworks")){
				if( req.urlTokens[3] == F("connect")){
					WiFiNetwork network;
					_restSerializationService->Json2Model(network, req.requestBody);
					network.SSID = req.urlTokens[2];
					station->connect(network, true);
					return  std::make_shared<ConnectionStatus>(station->getConnectionStatus());
				}
				if(req.urlTokens[3] == F("disconnect")){
					station->disconnect();
					return  std::make_shared<ConnectionStatus>(station->getConnectionStatus());
				}
				if(req.urlTokens[3] == F("save")) {//saving the network we currently connected to
					station->saveNetwork(station->getConnectionStatus().network);
					return std::make_shared<ListOfNetworks>(station->getSavedNetworks());
				}
			}
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 4) {
			if(req.urlTokens[1] == F("savedNetworks") && req.urlTokens[3] == F("forget")){
				station->forgetNetwork(req.urlTokens[2]);
				return std::make_shared<ListOfNetworks>(station->getSavedNetworks());
			}
		}
		Serial.println(F("__ERROR___: (inside StationController::handle)"));
		return nullptr;
	}
};

} /* namespace sm */

#endif /* STATIONMODULE_REST_STATIONCONTROLLER_H_ */
