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

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <DispatcherServlet.h>
#include <vector>

#include <memory>

//#include <typeinfo>

const char *ssid = "rina";//"AndroidAP";
const char *password = "1qwer5counterstrike";//"nakr0097";

ESP8266WebServer* server;

Garden* garden;

void initExecutionChains(DispatcherServlet& dispatcher) {
	Serial.println("############# creating new execution chain ###########");
	Serial.println("############# creating the PlantController ###########");
	Controller* plantCtrl = new PlantController(*garden);
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

	//init the model
	Serial.println("Creating the garden model");
	garden = new Garden();
	Serial.println("Printing the garden -");
	Serial.print("Number of plants:");
	Serial.println(garden->_plants.size());
	Serial.print("		plants -");
	for(int i = 0; i < garden->_plants.size(); i++)
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
	}


	//create the dispatcher model
	DispatcherServlet* dispatcher = new DispatcherServlet();

	//init the dispatcher with the handlers (execution chains) and the model
	dispatcher->_garden = garden;
	initExecutionChains(*dispatcher);

	//create a new server
	server = new ESP8266WebServer( *dispatcher ,80 );

	server->begin();
	Serial.println ( "HTTP server started" );

	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

	//Serial.println(typeid(dispatcher).name());

	shared_ptr<int> pi = make_shared<int>(4);
	Serial.println(*pi);


}

void loop ( void ) {
	server->handleClient();
}



