/*
 * NtpClientLib.h
 *
 *  Created on: 12 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWARETESTS_NTPCLIENT_SRC_NTPCLIENTLIB_H_
#define HARDWARETESTS_NTPCLIENT_SRC_NTPCLIENTLIB_H_

#include <time.h>

typedef enum {
    timeSyncd, // Time successfully got from NTP server
    noResponse, // No response from server
    invalidAddress // Address not reachable
} NTPSyncEvent_t;

class NTPClient {
	std::vector<std::function<void(NTPSyncEvent_t)>> callBacks;
public:
	NTPClient(){}
	~NTPClient(){}

	template<class... Args>
	void begin(Args ...args) {
	}

	void setInterval(int, int){
	}

	void onNTPSyncEvent(std::function<void(NTPSyncEvent_t)> nTPSyncEventFnc) {
		callBacks.push_back(nTPSyncEventFnc);
	}

	time_t getTime(){
		return time(0);
	}

	time_t getLastNTPSync() {
		return time(0);
	}

};

NTPClient NTP;



#endif /* HARDWARETESTS_NTPCLIENT_SRC_NTPCLIENTLIB_H_ */
