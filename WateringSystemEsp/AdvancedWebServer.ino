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
//#define DEBUG_MY_CODE

#include <DALModule/repositoryPattern/GardenUnitOfWork.h>
//#include <DALModule/serialization/cereal2.h>
//#include <DALModule/serialization/json2.h>
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

#include <Model/ObserverDesignPattern/Signal.hpp>
#include <Model/ObserverDesignPattern/Property.hpp>

#include <Model/ObserverDesignPattern/ObservableVector.h>

#include <chrono>

#include <TimeModule/timeService/TimeService.h>



const char *ssid = "rina";//"AndroidAP";//
const char *password = "1qwer5counterstrike";//"nakr0097";//

std::shared_ptr<ESP8266WebServer> server = nullptr;

template<typename MappingFileType>
std::shared_ptr<DAL::SerializationService2<mycereal::JsonSaveArchive<MappingFileType>, mycereal::JsonLoadArchive<MappingFileType>> > createSerializationServer(MappingFileType& mappingFile) {
	typedef mycereal::JsonLoadArchive<MappingFileType> LoadArchiveType;
	typedef mycereal::JsonSaveArchive<MappingFileType> SaveArchiveType;
	typedef DAL::SerializationService2<SaveArchiveType, LoadArchiveType > ServerType;

	auto loadArchive = std::make_shared<LoadArchiveType>(mappingFile);
	auto saveArchive = std::make_shared<SaveArchiveType>(mappingFile);
	auto serSevice =  std::make_shared<ServerType>(saveArchive, loadArchive);
	return serSevice;
}

void writeGardenToFlash(){
	{
		GardenModel::Garden garden = GardenModel::Garden();
		garden.name = String("FooooodGarden");
		std::shared_ptr<GardenModel::Sprinkler> sprinkler = std::make_shared<GardenModel::Sprinkler>();
		std::shared_ptr<GardenModel::SimpleProgram> simpleProgram = std::make_shared<GardenModel::SimpleProgram>();
		//std::shared_ptr<GardenModel::Plant> plant = std::make_shared<GardenModel::Plant>(sprinkler, simpleProgram);
		std::shared_ptr<GardenModel::Plant> plant = std::make_shared<GardenModel::Plant>(nullptr, simpleProgram);

		Serial.print("plant->_sprinkler is ");
		Serial.println(plant->_sprinkler == nullptr ? "null" : "not-null");

		plant->name = "Yellow Lily";
		sprinkler->id = 12;

		std::vector<GardenModel::Hour> hours = {GardenModel::Hour(11,12)};
		std::vector<GardenModel::Day> days = {GardenModel::Day(hours)};
		simpleProgram->timePattern = TimePattern(days);
		simpleProgram->id = 22;


		DAL::FlashMappingFile flashMappingFile;
		auto jsonSerializationService = createSerializationServer(flashMappingFile);
		garden._plants.push_back(plant);
		garden._sprinklers.push_back(sprinkler);
		garden._programs.push_back(simpleProgram);

		String str;
		jsonSerializationService->Model2Json(garden, str);
		Serial.println(str);
		bool result = SPIFFS.begin();

		{
			File f = SPIFFS.open("/f.txt","w");

			f.print(str);
			f.flush();
			f.close();
		}
		SPIFFS.end();

	}

}

/*bool readThefFile(String& str){
	bool result = SPIFFS.begin();
	Serial.print("SPIFFS opened: ");
	Serial.println(result);

	File f = SPIFFS.open("/f.txt","r");

	if (f) {
		Serial.println("we have a file:  /f.txt");
		str = f.readStringUntil('\0');
		f.flush();
		f.close();
		SPIFFS.end();
		return true;
	} else {
		f.close();
		SPIFFS.end();
		Serial.print("we dont have a file: /f.txt");
		return false;
	}
}*/

TS::TimeService timeService;

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

	/*Model::ObservableVector<std::shared_ptr<GardenModel::Plant> > plantz;
	typename Model::ObservableVector<std::shared_ptr<GardenModel::Plant> >::ConnectorFncType connectorFnc;
	connectorFnc = [](std::shared_ptr<GardenModel::Plant> plant, Model::Change change) {
		Serial.println("A plant was " + String( change == Model::Change::Added ? "Added" : "Deleted"));
		Serial.print("The plant id is");
		Serial.println(plant->id);
	};
	int slotId = plantz.on_change().connect(connectorFnc);
	plantz.push_back(std::make_shared<Plant>());
	std::shared_ptr<Plant> plantt = std::make_shared<Plant>();
	plantt->id = 22;
	plantz.push_back(plantt);

	Model::Property<int> intProp(3);
	std::function<void(int)> propConnFnB4 = [](int a) {Serial.print("Value changed from "); Serial.println(a);};
	std::function<void(int)> propConnFnOn = [](int a) {Serial.print("Value changed to "); Serial.println(a);};
	intProp.before_change().connect(propConnFnB4);
	intProp.on_change().connect(propConnFnOn);
	intProp = 5;
	intProp = 97;*/

	writeGardenToFlash();
//blabla
	MF::ModuleService mfs;
	config::moduleMap(mfs);

	bool success = mfs.startAllModules();
	if (!success)
		Serial.println("___CRITICAL ERROR___: Failed to start all modules");

	std::shared_ptr<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::FlashMappingFile>, mycereal::JsonLoadArchive<DAL::FlashMappingFile>>> serService = mfs.container->resolve<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::FlashMappingFile>, mycereal::JsonLoadArchive<DAL::FlashMappingFile>>>();
	Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@ what i belive to be the problematic line ");
	std::shared_ptr<Garden> garden = std::make_shared<Garden>();
	serService->Json2Model<Garden>(*garden, "{\"name\":\"FooooodGarden\",\"plants\":[{\"program\":22,\"id\":0,\"name\":\"Yellow Lily\"}],\"sprinklers\":[{\"id\":12,\"status\":\"Off\"}],\"programs\":[{\"id\":22,\"name\":\"not-set\",\"timePattern\":{\"days\":[{\"id\":0,\"hours\":[{\"id\":0,\"hour\":11,\"min\":12}]}]}}]}");
	String jsonGard;
	serService->Model2Json(*garden, jsonGard);
	Serial.println(jsonGard);
	Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@ AFTER what i belive to be the problematic line ");

	server = mfs.container->resolve<ESP8266WebServer>();

	server->begin();

	//initiate the TimeService (will be in the module later)
	timeService.initCurrentTime(myDate::my_clock::time_point{std::chrono::seconds{1507920363}});//18:46:03 UTC


	//timeService.

	Serial.println("HTTP server started");
	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	Serial.println(millis());
}

template <class T>
void printTime(T timeToday){
	Serial.print("The time is:");
	Serial.print(timeToday.hours().count());
	Serial.print(":");
	Serial.print(timeToday.minutes().count());
	Serial.print(":");
	Serial.print((long)timeToday.seconds().count());
	Serial.println();
}

void loop ( void ) {
//	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	server->handleClient();
	printTime(timeService.getCurrentDateTime());
	delay(1000 * 60 * 0.5);//0.5 min delay
}



