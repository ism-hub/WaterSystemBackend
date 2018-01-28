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
#include <myDate/myDate.h>

namespace TS {


class TimeService {
public:
	TimeService(){


	}

	virtual ~TimeService(){

	}

	void initCurrentTime(myDate::my_clock::time_point unixTimeNow){
		myDate::my_clock::setCurrentUnixTime(unixTimeNow);
	}

	myDate::my_clock::time_point getCurrentTime(){
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

};

} /* namespace TS */

#endif /* TIMEMODULE_TIMESERVICE_TIMESERVICE_H_ */
