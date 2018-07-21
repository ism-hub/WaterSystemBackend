/*
 * TimeService.h
 *
 *  Created on: 11 срхїз 2017
 *      Author: IsM
 */

#ifndef TIMEMODULE_TIMESERVICE_TIMESERVICE_H_
#define TIMEMODULE_TIMESERVICE_TIMESERVICE_H_

#include <chrono>
//#include <time.h>
//#include <date/date.h>
#include <myDate.h>

// *********** time stuff
// #include <TimeLib.h>
#include <NtpClient/src/NtpClientLib.h>
int8_t timeZone = 2;
int8_t minutesTimeZone = 0;
bool syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event


namespace TS {


class TimeService {
public:
	TimeService(){
		NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
			ntpEvent = event;
			syncEventTriggered = true;
		});
		NTP.begin ("pool.ntp.org", timeZone, true, minutesTimeZone);
		NTP.setInterval (10, 60*30);//every 10 sec on failure, once every 30 min on success
	}

	virtual ~TimeService(){

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

protected:
	// ************ time stuff
	void processSyncEvent (NTPSyncEvent_t ntpEvent) {
	    if (ntpEvent) {
	        Serial.print ("Time Sync error: ");
	        if (ntpEvent == noResponse)
	            Serial.println ("NTP server not reachable");
	        else if (ntpEvent == invalidAddress)
	            Serial.println ("Invalid NTP server address");
	    } else {
	        Serial.print ("Got NTP time: ");
	       // Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
	        initCurrentTime(myDate::my_clock::time_point{std::chrono::seconds{NTP.getLastNTPSync()}});
	    }
	    syncEventTriggered = false;
	}

};

} /* namespace TS */

#endif /* TIMEMODULE_TIMESERVICE_TIMESERVICE_H_ */
