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


//#include "rapidjson/document.h"
//#include "serialization/MyLightDocument.h"

//#include <typeinfo>


const char *ssid = "rina";//"AndroidAP";
const char *password = "1qwer5counterstrike";//"nakr0097";

std::shared_ptr<ESP8266WebServer> server = nullptr;

DAL::GardenUnitOfWork* gardenUnitOfWork;

/*void HelloWorldxXx(){
	Serial.println("calling the HWorld!! ");
}*/

/*void initExecutionChains(DispatcherServlet& dispatcher) {

	//	HelloWorldxXx();
	Serial.println("############# creating new execution chain ###########");
	Serial.println("############# creating the PlantController ###########");
	Controller* plantCtrl = new PlantController(*gardenUnitOfWork);
	HandlerExecutionChain* handlerExecutionChain = new HandlerExecutionChain(plantCtrl);

	//Serial.println("############# trying to do: handlerExecutionChain->getController() ###########");
	//Controller* ctrl = handlerExecutionChain->getController();

	Serial.println("############# adding the interceptor ###########");
	//adding the interceptor
	JsonGardenVisitor* jsonVisitor = new JsonGardenVisitor();
	JsonHandlerInterceptor* jsonInterceptor = new JsonHandlerInterceptor(*jsonVisitor);
	handlerExecutionChain->addInterceptor(jsonInterceptor);

	Serial.println("############# inserting the exe chain into the dispatcher ###########");
	dispatcher._handlerExecutionChains.push_back(handlerExecutionChain);

}*/

/*
enum class sfinae {};
static const sfinae sfinae2 = {};

template <typename T, typename std::enable_if<std::numeric_limits<T>::is_integer,  sfinae>::type = sfinae2 > inline
static void foo(T a){
	Serial.println("called the int ver");
	Serial.println(a);
}

template <typename T, typename std::enable_if<!std::numeric_limits<T>::is_integer, sfinae >::type = sfinae2>inline
static void foo(T a){
	Serial.println("called the float ver");
	//Serial.println(a);
}
*/
//static void foo(float a){
//	Serial.println("called the float ver");
//	Serial.println(a);
//}


/*

template<typename, typename T>
struct has_serialize {
    static_assert(
        std::integral_constant<T, false>::value,
        "Second template parameter needs to be of function type.");
};

template<typename C, typename Ret, typename... Args>
struct has_serialize<C, Ret(Args...)> {
private:
    template<typename T>
    static constexpr auto check(T*)-> typename std::is_same< decltype( std::declval<T>().serialize( std::declval<Args>()... ) ), Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
																																	>::type;  // attempt to call it and see if the return type is correct

    template<typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

struct X {
     void serialize(const int&) { return ; }
};

struct Y : X {};

*/
/*
class A {
public:
	int x=3;
	char ca='a';
//	char const * lala = "Trololololol";

	template <class Archive>
	void save(Archive& archive) const{
		archive(CEREAL2_NVP(x));
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(CEREAL2_NVP(x));
	}

	void start(){
		Serial.println("called A::start");
	}
};*/

/*struct membuf : std::streambuf
{
	membuf(char* begin, char* end){
		this->setg(begin, begin, end);
	}
};*/



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

	//init the model
/*	Serial.println("Creating the garden model");
	gardenUnitOfWork = new DAL::GardenUnitOfWork(*(new DAL::GardenModelContext()));
	if(gardenUnitOfWork->_gardenContext._garden == nullptr){
		Serial.println("_garden is nullptr, inserting new garden");
		gardenUnitOfWork->_gardenContext._garden = make_shared<Garden>();
	}*/

	//garden = new Garden();
//	Serial.println("Printing the garden -");
//	Serial.print("Number of plants:");
	//Serial.println(garden->_plants.size());
//	Serial.print("		plants -");
	/*for(int i = 0; i < garden->_plants.size(); i++)
	{
		Serial.println(i);
		weak_ptr<Plant> plant = garden->_plants[i];
		if(plant.expired())
			Serial.println("for some reason the plant pointer is expired");
		weak_ptr<Sprinkler> sprinkler = plant.lock()->_sprinkler;
		if(sprinkler.expired())
			Serial.println("for some reason the sprinkler pointer is expired");
		Serial.print("Plant.sprinkler._id - ");
		Serial.println(sprinkler.lock()->_id);
		Serial.println("Plant.sprinkler._name - " + sprinkler.lock()->_name);
		Serial.print("Plant.sprinkler._name - ");
		Serial.println(sprinkler.lock()->_status == Sprinkler::On ? "On" : "Off");
	}*/


	//create the dispatcher model
//	DispatcherServlet* dispatcher = new DispatcherServlet();

	//init the dispatcher with the handlers (execution chains) and the model
	//dispatcher->_garden = gardenUnitOfWork->_gardenContext._garden.get();
	//initExecutionChains(*dispatcher);

	//create a new server
	//server = new ESP8266WebServer( *dispatcher ,80 );

	//server->begin();
//	Serial.println ( "HTTP server started" );

//	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

	//Serial.println(typeid(dispatcher).name());

	/*shared_ptr<int> pi = make_shared<int>(4);
	Serial.println(*pi);

	Serial.print("has_serialize<X, void(const int&)>::value : ");
	Serial.println(has_serialize<X, void(const int&)>::value);

	Serial.print("has_serialize<Y, void(const int&)>::value : ");
	Serial.println(has_serialize<Y, void(const int&)>::value);

	Serial.print("has_serialize<ESP8266WebServer, void(const int&)>::value : ");
	Serial.println(has_serialize<ESP8266WebServer, void(const int&)>::value);
*/
	//111111111111111111111111111

	/*int x=6666;
	auto nvp = CEREAL2_NVP(x);

	Serial.println(nvp.value);
*/
//	foo(3);
//	foo('a');
//	foo(3.1);
/*
	rapidjson::StringBuffer ss;
	rapidjson::Writer<rapidjson::StringBuffer> writer2(ss);

	cereal2::JSONOutputArchive& archive2 = *(new cereal2::JSONOutputArchive(writer2));
	Garden& gardenn = *(gardenUnitOfWork->_gardenContext._garden.get());
	archive2(CEREAL2_NVP(gardenn));
	archive2.~JSONOutputArchive();
	Serial.println(ss.GetString());


	rapidjson::StringBuffer s;
	rapidjson::Writer<rapidjson::StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("hello");
	writer.String("world");
	writer.Key("t");
	writer.Bool(true);
	writer.Key("f");
	writer.Bool(false);
	writer.Key("n");
	writer.Null();
	writer.Key("i");
	writer.Uint(123);
	//writer.Key("pi");
	//writer.Double(3.1416);
	writer.Key("a");
	writer.StartArray();
	for (unsigned i = 0; i < 4; i++)
		writer.Uint(i);
	writer.EndArray();
	writer.EndObject();

	Serial.println(s.GetString());
	//cout << s.GetString() << endl;

	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

	A a;
	//rapidjson::StringStream jsonInput("{\"a\":{\"x\":113}}");
	rapidjson::StringStream jsonInput("{\"garden\":{\"_plants\":[{\"_sprinkler\":{\"_id\":123}}]}}");

	//membuf membufStr(jsonInput, jsonInput + sizeof(jsonInput));
	//std::istream istreamChar(&membufStr);

	//std::cout << "dsdsdsd" << std::endl;

	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
	Serial.println("creating JSONInputArchive");
	//cereal2::JSONInputArchive readArchive(jsonInput);//(istreamChar);
	Garden garden;
	cereal2::JSONInputArchive readArchive(jsonInput);//(istreamChar);
	Serial.println("end creating JSONInputArchive");
	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

	Serial.println("calling readArchive(garden);");
	readArchive(garden);

	Serial.println("@@@@@@@@@@");
	Serial.println(garden._plants[1]->_sprinkler->_id);

	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

	//rapidjson2::MyLightDocument doc;
	//doc.parse();

//	rapidjson::Document document;
//	document.Parse("{\"a\": {\"x\":113}}");
*/

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



