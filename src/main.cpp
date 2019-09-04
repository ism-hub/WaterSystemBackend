
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <vector>
#include <memory>
#include <type_traits>
#include <limits>
#include <ModuleFramework/ModuleFramework.hpp>
#include <config/moduleMap.h>
#include <chrono>
#include <TimeModule/timeService/TimeService.h>
#include <ServiceFramework/ServiceFrameWork.h>


std::shared_ptr<ESP8266WebServer> server = nullptr;
std::shared_ptr<sfwk::ServiceFrameWork> serviceFrameWork = nullptr;
std::shared_ptr<sched::SchedulerService> scheduler = nullptr;

void setup ( void ) {
	//TODO: add this to the ap station module
	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	Serial.begin ( 9600 );//Serial.begin ( 115200 );
	WiFi.persistent(false);
	WiFi.mode(WIFI_AP_STA);

	//TODO: open a mdns module and add it to there
	String mdnsName = F("watersystem");
	if ( MDNS.begin(mdnsName) ) {
		 Serial.print("mDNS responder started: http://");
		Serial.print(mdnsName);
		Serial.println(".local");
		//Serial.println("MDNS responder started");
		//MDNS.setHostname("watersystem");
 	}

	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	MF::ModuleService mfs;
	config::moduleMap(mfs);

	auto success = mfs.startAllModules();
	if (success != MF::ErrorCode::NO_ERROR)
		Serial.println("___CRITICAL ERROR___: Failed to start all modules");
	Serial.println("Done loading all the modules");
	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());


	serviceFrameWork = mfs.getContainer()->resolve<sfwk::ServiceFrameWork>();
	server = mfs.getContainer()->resolve<ESP8266WebServer>();
	server->begin();
	Serial.println("HTTP server started");
	scheduler = mfs.getContainer()->resolve<sched::SchedulerService>();

	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	Serial.println(millis());
}

void loop ( void ) {
	//MDNS.update(); //?
	server->handleClient();
	scheduler->execute();

	//printing the heap status every 10 min
	static int last = 0;
	static bool printTimeNow = true;
	if (((millis () - last) > (1000 * 60 * 1)) || printTimeNow ) {//5 minute has passed
		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
		//printTime(timeService->getCurrentDateTime());
		last = millis();
		printTimeNow = false;
	}

}