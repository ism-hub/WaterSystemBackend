/*
 * TimeService.h
 *
 *  Created on: 11 ����� 2017
 *      Author: IsM
 */

#ifndef TIMEMODULE_TIMESERVICE_TIMESERVICE_H_
#define TIMEMODULE_TIMESERVICE_TIMESERVICE_H_

#include <ServiceFramework/IService.h>
#include <TimeModule/DAL/NtpSettingsContex.h>

#include <chrono>
//#include <time.h>
//#include <date/date.h>
#include <myDate\myDate.h>

// *********** time stuff
// #include <TimeLib.h>
#include <NtpClientLib.h>



namespace tsm {
bool syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event


class TimeService : public sfwk::IService {
	std::shared_ptr<NtpSettingsContex> ntpSettingsContex;
public:
	TimeService(std::shared_ptr<NtpSettingsContex> ntpSettingsContex) : sfwk::IService(F("TimeService")), ntpSettingsContex(ntpSettingsContex){
	//	Serial.println("INSIDE  TimeService CTOR");
		NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
			ntpEvent = event;
			syncEventTriggered = true;
		});
		NTP.begin (ntpSettingsContex->get()->serverIP, ntpSettingsContex->get()->timeZone, ntpSettingsContex->get()->daylight, ntpSettingsContex->get()->minutesTimeZone);
		std::chrono::seconds intervalOnFailure = ntpSettingsContex->get()->intervalOnFailure;
		std::chrono::seconds intervalOnSuccess = ntpSettingsContex->get()->intervalOnSuccess;
		NTP.setInterval (intervalOnFailure.count(), intervalOnSuccess.count());//every 10 sec on failure, once every 10 min on success
//		Serial.println("END INSIDE  TimeService CTOR");
	}

	 ~TimeService(){

	}

	void initCurrentTime(myDate::my_clock::time_point unixTimeNow){
		myDate::my_clock::setCurrentUnixTime(unixTimeNow);
	}

	void initCurrentTime(){
		time_t timet = NTP.getTime();
		myDate::my_clock::setCurrentUnixTime(myDate::my_clock::time_point{std::chrono::seconds{timet}});
	}

	myDate::my_clock::time_point getCurrentTime(){
		if(syncEventTriggered)
			processSyncEvent(ntpEvent);

		return myDate::my_clock::now();
	}

	myDate::time_of_day<myDate::my_clock::duration> getCurrentDateTime(){
		//initCurrentTime(myDate::my_clock::time_point{myDate::my_clock::duration{24*60*60 + 15*60*60 + 49*60 + 30 }});
		auto currentTime = getCurrentTime();
		auto currentDays = std::chrono::time_point_cast<myDate::days>(currentTime);
		auto secFromDayStart = currentTime - currentDays;
		auto timeToday = myDate::make_time(secFromDayStart);
		//auto ymd = myDate::year_month_day{currentDays};
		//return std::pair{ymd, timeToday};
		return timeToday;
	}

	int StartService(){
		return 0;
	}

	int StopService(){
		return 0;
	}

protected:
	// ************ time stuff
	void processSyncEvent (NTPSyncEvent_t ntpEvent) {
	    if (ntpEvent) {
	        Serial.print(F("Time Sync error: "));
	        if (ntpEvent == noResponse)
	            Serial.println(F("NTP server not reachable"));
	        else if (ntpEvent == invalidAddress)
	            Serial.println(F("Invalid NTP server address"));
	    } else {
	        Serial.print(F("Got NTP time: "));
	       // Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
	        initCurrentTime(myDate::my_clock::time_point{std::chrono::seconds{NTP.getLastNTPSync()}});
	    }
	    syncEventTriggered = false;
	}

};

} /* namespace TS */

#endif /* TIMEMODULE_TIMESERVICE_TIMESERVICE_H_ */
