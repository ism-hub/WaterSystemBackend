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

	/*Serial.println("init Calling to the get controller before the handle client");
	Serial.println("init getting the dispatcher servlet");
	DispatcherServlet* dispacher2 = &dispatcher;
	Serial.println("init getting the handlerExecutionChains");
	vector<HandlerExecutionChain*>* exeChains = dispacher2->_handlerExecutionChains;
	Serial.println("init getting the only ExecutionChain we have");
	HandlerExecutionChain* exeChain = (*exeChains)[0];
	Serial.println("init getting the controller");
	Controller* ctrler = exeChain->getController();*/
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
	garden = new Garden();

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
	//server._dispatcherServlet._handlerExecutionChains


	Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());
}

void loop ( void ) {
	/*Serial.println("Calling to the get controller before the handle client");
	Serial.println("getting the dispatcher servlet");
	DispatcherServlet* dispacher2 = &(server->_dispatcherServlet);
	Serial.println("getting the handlerExecutionChains");
	HandlerExecutionChain** exeChains = dispacher2->_handlerExecutionChains;
	Serial.println("getting the only ExecutionChain we have");
	HandlerExecutionChain* exeChain = *exeChains;
	Serial.println("getting the controller");
	Controller* ctrler = exeChain->getController();

	Serial.println("success, now calling the server->handleClient(); ");*/
	server->handleClient();
	//delay(2000);
}



