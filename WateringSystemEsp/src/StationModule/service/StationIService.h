/*
 * StationISerivice.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_SERVICE_STATIONISERVICE_H_
#define STATIONMODULE_SERVICE_STATIONISERVICE_H_

#include <ServiceFramework/IService.h>

#include <StationModule/model/Station.h>
#include <StationModule/DAL/StationContex.h>

namespace sm {

class StationIService : public sfwk::IService{
	std::shared_ptr<StationContex> stationContex;
public:
	StationIService(std::shared_ptr<StationContex> stationContex) :
		sfwk::IService(F("StationIService")), stationContex(stationContex)
	{
		//sm::WiFiNetwork wifiNet;
		//wifiNet.SSID = F("Ratatata");
		//wifiNet.password = F("c0un73rs7r1k3");
		// std::shared_ptr<Station> station = stationContex->get();
		//station->setScheduler(scheduler);
		//Serial.println("^^^^^^^^^^^^^^^^^^^^^^^ before station->connect(wifiNet, false);");
		//station->connect(wifiNet, false);
	}
	 ~StationIService() {
//		Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~ StationIService DTOR");
	}

	int StartService() {
		std::shared_ptr<Station> station = stationContex->get();
		station->connectAuto();
		return 0;
	}

	int StopService() {
		Serial.println(F("Stopping StationIService server"));
		return 0;
	}

	std::shared_ptr<Station> getStation(){
		return stationContex->get();
	}

	int saveStation(){
		return stationContex->save();
	}

};

} /* namespace sm */

#endif /* STATIONMODULE_SERVICE_STATIONISERVICE_H_ */
