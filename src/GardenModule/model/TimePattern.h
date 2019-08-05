/*
 * TimePattern.h
 *
 *  Created on: 7 срхїз 2017
 *      Author: IsM
 */

#ifndef MODEL_TIMEPATTERN_H_
#define MODEL_TIMEPATTERN_H_

#include <vector>
#include <algorithm>
#include <DALFramework/serialization/NameValuePair.h>

namespace garden {

class Hour{//24 format

public:
	int id = 0;
	int hour = 0;
	int min = 0;
	int sec = 0;
	int duration = 30;//the amount of time which the sprinkler will stay open

	Hour(){
#ifdef DEBUG_MY_CODE
		Serial.println("Hour CTOR");
#endif

	}

	Hour(int hour, int min) : hour(hour), min(min){
#ifdef DEBUG_MY_CODE
		Serial.println("Hour CTOR");
#endif
	}

	~Hour(){
#ifdef DEBUG_MY_CODE
		Serial.println("Hour DTOR");
#endif
	}


	bool operator==(const Hour& rhs) const{
		return (hour == rhs.hour && min == rhs.min && duration == rhs.duration);
	}

	bool operator!=(const Hour& rhs) const{
		return !(*this == rhs);
	}

	bool operator<(const Hour& rhs) const{
		if(hour < rhs.hour)
			return true;
		if(hour == rhs.hour)
			if(min < rhs.min)
				return true;
		return false;
	}

	//the minutes between them
	int operator-(const Hour& rhs) const{
		int minDiff = (hour - rhs.hour)*60*60 + (min - rhs.min)*60 + sec - rhs.sec;
		return minDiff;
	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const {
		archive.addProperties(mappingFile, MACRO_NVP(id), MACRO_NVP(hour), MACRO_NVP(min), MACRO_NVP(duration));
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
		archive.loadProperties(mappingFile, MACRO_NVP(id), MACRO_NVP(hour), MACRO_NVP(min), MACRO_NVP(duration));
	}
};

class Day{

public:
	int id = 0;
	std::vector<Hour> hours;

	Day(){
#ifdef DEBUG_MY_CODE
		Serial.println("Day CTOR");
#endif
		}

	Day(std::vector<Hour> houres) : hours(houres){
#ifdef DEBUG_MY_CODE
		Serial.println("Day CTOR");
#endif
		std::sort(this->hours.begin(), this->hours.end());
	}

	Day(std::vector<Hour>&& houres) : hours(std::move(houres)){
#ifdef DEBUG_MY_CODE
		Serial.println("Day CTOR");
#endif
		std::sort(this->hours.begin(), this->hours.end());
	}

	~Day(){
#ifdef DEBUG_MY_CODE
		Serial.println("Day DTOR");
#endif
	}

	const std::vector<Hour>& getHoures() const{
		return hours;
	}

	/*void eraseHoure(Hour& houre){
		houres.erase(std::remove(houres.begin(), houres.end(), houre), houres.end());
	}*/

	bool operator==(const Day& rhs) const {
		if (hours.size() != rhs.hours.size()) {
			return false;
		}
		//check hour by hour
		for (unsigned int i = 0; i < hours.size(); i++) {//the hours in the day are ordered
			if(hours[i] != rhs.hours[i])
				return false;
		}
		return true;
	}

	bool operator!=(const Day& rhs) const {
		return !(*this == rhs);
	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const {
		archive.addProperties(mappingFile, MACRO_NVP(id), MACRO_NVP(hours));
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
		archive.loadProperties(mappingFile, MACRO_NVP(id), MACRO_NVP(hours));
	}
};

class TimePattern{

public:
	std::vector<Day> days;

	TimePattern(){
#ifdef DEBUG_MY_CODE
		Serial.println("TimePattern CTOR");
#endif
		}

	TimePattern(std::vector<Day> days) : days(days){
#ifdef DEBUG_MY_CODE
		Serial.println("TimePattern CTOR");
#endif
	}

	TimePattern(std::vector<Day>&& days) : days(std::move(days)){
#ifdef DEBUG_MY_CODE
		Serial.println("TimePattern CTOR");
#endif
	}

	~TimePattern(){
#ifdef DEBUG_MY_CODE
		Serial.println("TimePattern DTOR");
#endif
	}

	bool operator!=(const TimePattern& rhs) const {
		return !(*this == rhs);
	}

	bool operator==(const TimePattern& rhs) const {
		if (days.size() != rhs.days.size()) {
			return false;
		}
		for (unsigned int i = 0; i < days.size(); i++) {
			if (days[i] != rhs.days[i])
				return false;
		}
		return true;
	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const {
#ifdef DEBUG_MY_CODE
		Serial.println("before TimePattern save");
#endif
		archive.addProperties(mappingFile, MACRO_NVP(days));
#ifdef DEBUG_MY_CODE
		Serial.println("after TimePattern save");
#endif
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
#ifdef DEBUG_MY_CODE
		Serial.println("before TimePattern load");
#endif
		archive.loadProperties(mappingFile, MACRO_NVP(days));
#ifdef DEBUG_MY_CODE
		Serial.println("after TimePattern load");
#endif
	}
};

} /* namespace GardenModel */

#endif /* MODEL_TIMEPATTERN_H_ */
