/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 java -jar EspStackTraceDecoder.jar C:/Users/IsM/AppData/Local/Arduino15/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-addr2line C:/Programming/Cworkspace/WateringSystemEsp/Release/WateringSystemEsp.elf ./bla/dump.txt

 */


#include <DALModule/repositoryPattern/GardenUnitOfWork.h>
#include <DALModule/serialization/cereal2.h>
#include <DALModule/serialization/json2.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <httpModule/dispatcher/DispatcherServlet.h>

#include <vector>

#include <memory>

#include <type_traits>
#include <limits>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <ModuleFramework/ModuleService.h>
#include <ModuleFramework/SomeModule.h>
#include <ModuleFramework/Container/Container.h>

#include <config/moduleMap.h>


const char *ssid = "rina";//"AndroidAP";
const char *password = "1qwer5counterstrike";//"nakr0097";

std::shared_ptr<ESP8266WebServer> server = nullptr;

void writeGardenToFlash(){
	{

		bool result = SPIFFS.begin();
		Serial.print("SPIFFS opened: ");
		Serial.println(result);

		if (SPIFFS.exists("/f.txt")) {
			SPIFFS.end();
			return;
		}


		GardenModel::Garden garden = GardenModel::Garden();
		std::shared_ptr<GardenModel::Sprinkler> sprinkler = std::make_shared<GardenModel::Sprinkler>();
		std::shared_ptr<GardenModel::Plant> plant = std::make_shared<GardenModel::Plant>(sprinkler);
		plant->name = "Yellow Lily";
		sprinkler->id = 12;

		std::shared_ptr<cereal2::JSONInputArchive> 	inputArchive = std::make_shared<cereal2::JSONInputArchive>();
		rapidjson::StringBuffer						_stringBuffer;
		std::shared_ptr<cereal2::JSONOutputArchive> outputArchive = std::make_shared<cereal2::JSONOutputArchive>();
		DAL::JsonSerializationService jsonSerializationService(inputArchive, outputArchive);
		garden._plants.push_back(plant);
		Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		String str = jsonSerializationService.modelToJson(garden);
		Serial.println("The string we writing to the file");
		Serial.println(str);

		{
			File f = SPIFFS.open("/f.txt", "w");

			f.print(str);
			f.flush();
			f.close();
		}
		SPIFFS.end();

	}

	Serial.println("SPIFFS closing: ");

}

bool readThefFile(String& str){
	bool result = SPIFFS.begin();
	Serial.print("SPIFFS opened: ");
	Serial.println(result);

	File f = SPIFFS.open("/f.txt", "r");

	if (f) {
		Serial.println("we have a file:  /f.txt");
		str = f.readStringUntil('\0');
		f.flush();
		f.close();
		SPIFFS.end();
		return true;
	} else {
		Serial.print("we dont have a file:  /f.txt");
		f.close();
		SPIFFS.end();
		return false;
	}
}

void setup ( void ) {
	Serial.begin ( 115200 );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

	if ( MDNS.begin ( "esp8266" ) ) {
		Serial.println ( "MDNS responder started" );
	}

	writeGardenToFlash();

	MF::ModuleService mfs;
	config::moduleMap(mfs);

	bool success = mfs.startAllModules();
	if (!success)
		Serial.println("___CRITICAL ERROR___: Failed to start all modules");

	server = mfs.container->resolve<ESP8266WebServer>();

	server->begin();
	Serial.println ( "HTTP server started" );
	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
}

void loop ( void ) {
//	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	server->handleClient();
	//delay(1000);
}



