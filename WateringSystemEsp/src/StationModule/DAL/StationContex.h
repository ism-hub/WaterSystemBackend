/*
 * StationContex.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_DAL_STATIONCONTEX_H_
#define STATIONMODULE_DAL_STATIONCONTEX_H_

#include <FS.h>
#include <DALFramework/SPIFFModelContex.h>
#include <DALModule/DALModule.h>

#include <StationModule/model/Station.h>
#include <StationModule/DAL/StationFlashSerializationService.h>

namespace sm {

typedef dalfw::SPIFFModelContex<StationFlashSerializationService, Station> StationSPIFFContex;

class StationContex : public StationSPIFFContex {
	std::shared_ptr<sched::SchedulerService> scheduler = nullptr;
public:
	StationContex(std::shared_ptr<StationFlashSerializationService> jsonSerializationService, std::shared_ptr<sched::SchedulerService> scheduler, String savePath) :
		StationSPIFFContex(jsonSerializationService, savePath), scheduler(scheduler)
	{}
	 ~StationContex() {
	//	Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~ StationContex DTOR");
	}

	std::shared_ptr<Station> get() override {
		std::shared_ptr<Station> station = StationSPIFFContex::get();
		station->setScheduler(scheduler);
		return station;
	}

};

} /* namespace sm */

#endif /* STATIONMODULE_DAL_STATIONCONTEX_H_ */
