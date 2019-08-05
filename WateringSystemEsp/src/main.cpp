
//#define DEBUG_MY_CODE

//#include <GardenModule/DAL/repositoryPattern/GardenUnitOfWork.h>
//#include <DALModule/serialization/cereal2.h>
//#include <DALModule/serialization/json2.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include <HttpFramework/dispatcher/DispatcherServlet.h>

#include <vector>
#include <memory>
#include <type_traits>
#include <limits>

#include <ModuleFramework/ModuleService.h>
#include <ModuleFramework/ModuleService.h>
#include <ModuleFramework/Container/Container.h>
#include <config/moduleMap.h>

//#include <Model/ObserverDesignPattern/Signal.hpp>
//#include <Model/ObserverDesignPattern/Property.hpp>

//#include <Model/ObserverDesignPattern/ObservableVector.h>

#include <chrono>
#include <TimeModule/timeService/TimeService.h>

#include <ServiceFramework/ServiceFrameWork.h>

//#include <SPI.h>

//#include <SPIService.h>


// const char *ssid = "Ratatata";//"AndroidAP";//
// const char *password = "c0un73rs7r1k3";//"nakr0097";//

std::shared_ptr<ESP8266WebServer> server = nullptr;
std::shared_ptr<sfwk::ServiceFrameWork> serviceFrameWork = nullptr;
std::shared_ptr<sched::SchedulerService> scheduler = nullptr;

//template<typename MappingFileType>
//std::shared_ptr<DAL::SerializationService2<mycereal::JsonSaveArchive<MappingFileType>, mycereal::JsonLoadArchive<MappingFileType>> > createSerializationServer(MappingFileType& mappingFile) {
//	typedef mycereal::JsonLoadArchive<MappingFileType> LoadArchiveType;
//	typedef mycereal::JsonSaveArchive<MappingFileType> SaveArchiveType;
//	typedef DAL::SerializationService2<SaveArchiveType, LoadArchiveType > ServerType;
//
//	auto loadArchive = std::make_shared<LoadArchiveType>(mappingFile);
//	auto saveArchive = std::make_shared<SaveArchiveType>(mappingFile);
//	auto serSevice =  std::make_shared<ServerType>(saveArchive, loadArchive);
//	return serSevice;
//}

//void writeGardenToFlash(){
//	{
//		GardenModel::Garden garden = GardenModel::Garden();
//		garden.name = String("FooooodGarden");
//		std::shared_ptr<GardenModel::Sprinkler> sprinkler = std::make_shared<GardenModel::Sprinkler>();
//		std::shared_ptr<GardenModel::SimpleProgram> simpleProgram = std::make_shared<GardenModel::SimpleProgram>();
//		std::shared_ptr<GardenModel::Plant> plant = std::make_shared<GardenModel::Plant>(sprinkler, simpleProgram);
//		//std::shared_ptr<GardenModel::Plant> plant = std::make_shared<GardenModel::Plant>(nullptr, simpleProgram);
//
//		Serial.print("plant->_sprinkler is ");
//		Serial.println(plant->_sprinkler == nullptr ? "null" : "not-null");
//
//		plant->name = "Yellow Lily";
//		plant->id = 1;
//		sprinkler->id = 1;
//		sprinkler->status = GardenModel::Sprinkler::Off;
//
//		std::vector<GardenModel::Hour> hours = {GardenModel::Hour(13,18)};
//		std::vector<GardenModel::Day> days = {GardenModel::Day(hours)};
//		simpleProgram->timePattern = TimePattern(days);
//		simpleProgram->id = 1;
//
//		//Adding another plant with sprinkler and program
//		std::shared_ptr<GardenModel::Sprinkler> sprinkler2 = std::make_shared<GardenModel::Sprinkler>();
//		std::shared_ptr<GardenModel::SimpleProgram> simpleProgram2 = std::make_shared<GardenModel::SimpleProgram>();
//		std::shared_ptr<GardenModel::Plant> plant2 = std::make_shared<GardenModel::Plant>(sprinkler2, simpleProgram2);
//
//		plant2->name = "Yellow Lily2";
//		plant2->id = 2;
//		sprinkler2->id = 2;
//		sprinkler2->status = GardenModel::Sprinkler::Off;
//
//		std::vector<GardenModel::Hour> hours2 = {GardenModel::Hour(13,20)};
//		std::vector<GardenModel::Day> days2 = {GardenModel::Day(hours2)};
//		simpleProgram2->timePattern = TimePattern(days2);
//		simpleProgram2->id = 2;
//
//
//
//		DAL::FlashMappingFile flashMappingFile;
//		auto jsonSerializationService = createSerializationServer(flashMappingFile);
//		garden._sprinklers.push_back(sprinkler);
//		garden._sprinklers.push_back(sprinkler2);
//		garden._programs.push_back(simpleProgram);
//		garden._programs.push_back(simpleProgram2);
//		garden._plants.push_back(plant);
//		garden._plants.push_back(plant2);
//
//		String str;
//		jsonSerializationService->Model2Json(garden, str);
//		Serial.println(str);
//		bool result = SPIFFS.begin();
//
//		{
//			File f = SPIFFS.open("/f.txt","w");
//
//			f.print(str);
//			f.flush();
//			f.close();
//		}
//		SPIFFS.end();
//
//	}
//
//}

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

//TS::TimeService timeService;
std::shared_ptr<tsm::TimeService> timeService;

void setup ( void ) {
	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	Serial.begin ( 9600 );//Serial.begin ( 115200 );
	WiFi.persistent(false);
	WiFi.mode(WIFI_AP_STA);
	/*WiFi.begin ( ssid, password);
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}


	Serial.println ( "" );
	Serial.print("Connected to ");
	Serial.println ( ssid );
	Serial.print("IP address: " );
	Serial.println ( WiFi.localIP() );*/

	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

	MDNS.setHostname("watersystem");
	if ( MDNS.begin ("watersystem") ) {
		Serial.println("MDNS responder started");
		MDNS.setHostname("watersystem");
	}
	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
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

	// %%%%%%%%%%%%% writeGardenToFlash();
//blabla
	MF::ModuleService mfs;
	config::moduleMap(mfs);

	bool success = mfs.startAllModules();
	if (!success)
		Serial.println("___CRITICAL ERROR___: Failed to start all modules");

	Serial.println("MMMMMMM Done loading all the modules");
	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

	//std::shared_ptr<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::FlashMappingFile>, mycereal::JsonLoadArchive<DAL::FlashMappingFile>>> serService = mfs.container->resolve<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::FlashMappingFile>, mycereal::JsonLoadArchive<DAL::FlashMappingFile>>>();
	//Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@ what i belive to be the problematic line ");
	//std::shared_ptr<Garden> garden = std::make_shared<Garden>();
	//serService->Json2Model<Garden>(*garden, "{\"name\":\"FooooodGarden\",\"plants\":[{\"program\":22,\"id\":0,\"name\":\"Yellow Lily\"}],\"sprinklers\":[{\"id\":12,\"status\":\"Off\"}],\"programs\":[{\"id\":22,\"name\":\"not-set\",\"timePattern\":{\"days\":[{\"id\":0,\"hours\":[{\"id\":0,\"hour\":11,\"min\":12}]}]}}]}");
	//String jsonGard;
	//serService->Model2Json(*garden, jsonGard);
	//Serial.println(jsonGard);
	//Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@ AFTER what i belive to be the problematic line ");

	serviceFrameWork = mfs.container->resolve<sfwk::ServiceFrameWork>();

	server = mfs.container->resolve<ESP8266WebServer>();

	server->begin();

	scheduler = mfs.container->resolve<sched::SchedulerService>();



	Serial.println("HTTP server started");
	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	Serial.println(millis());
}


template <class T>
void printTime(T timeToday){
	Serial.print(F("The time is:"));
	Serial.print(timeToday.hours().count());
	Serial.print(":");
	Serial.print(timeToday.minutes().count());
	Serial.print(":");
	Serial.print((long)timeToday.seconds().count());
	Serial.println();
}


void loop ( void ) {
	//MDNS.begin ( "watersystem" );
	MDNS.update();
//	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	server->handleClient();

	scheduler->execute();

	static int last = 0;
	static bool printTimeNow = true;
	if (((millis () - last) > (1000 * 60 * 1)) || printTimeNow ) {//5 minute has passed
		Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
		//printTime(timeService->getCurrentDateTime());
		last = millis();
		printTimeNow = false;
	}

}



