/*
 * Station.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_MODEL_STATION_H_
#define STATIONMODULE_MODEL_STATION_H_

#include <ESP8266WiFi.h>

#include <vector>
#include <algorithm>    // std::find
#include <functional>   // std::bind

#include <StationModule/model/IStationAcceptable.h>
#include <StationModule/model/WiFiNetwork.h>
#include <StationModule/model/ConnectionStatus.h>

#include <SchedulerService.h>

namespace sm {

class Station : public IStationAcceptable{
	//networks you saved the password to or networks without password you saved (if you saved a password for a network and now that network is open we wont connect)
	std::shared_ptr<sched::SchedulerService> scheduler = nullptr;
	std::vector<WiFiNetwork> savedNetworks;
	mutable ConnectionStatus connectionStatus;
	std::shared_ptr<Task> tsk = nullptr;// the task we added to the service
	stf::CachedValue<std::vector<WiFiNetwork>> nearbyNetworksCache;

public:
	Station(std::shared_ptr<sched::SchedulerService> scheduler, std::vector<WiFiNetwork> savedNetworks) :
		scheduler(scheduler), savedNetworks(savedNetworks), nearbyNetworksCache(std::chrono::seconds{20}, std::bind(&Station::scanInner, this, _1)) {
	//	Serial.println("------------------- Station CTOR1");
	}

	Station() : nearbyNetworksCache(std::chrono::seconds{20}, std::bind(&Station::scanInner, this, _1)){
//		Serial.println("------------------- Station CTOR2");
	}//for the serialization.. we need to add the 'shcheduler' somehow

	void setScheduler(std::shared_ptr<sched::SchedulerService> scheduler) {
		this->scheduler = scheduler;
	}

	virtual ~Station() {
	//	Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Station DTOR");
	}

	bool isInSavedNetworks(const WiFiNetwork& network){
		auto p = std::find(savedNetworks.begin(), savedNetworks.end(), network);
		return p != savedNetworks.end();
	}

	bool isNearbyNetwork(const WiFiNetwork& network, bool useCache = true){
		if(!useCache)
			nearbyNetworksCache.forceUpdate();
		auto nearbyNets = nearbyNetworksCache.get();
		auto p = std::find(nearbyNets.begin(), nearbyNets.end(), network);
		return p != savedNetworks.end();
	}

	ConnectionStatus& getConnectionStatus() const {
		updateConnectionStatus();
		return connectionStatus;
	}

	/*
	 * returns smart cache value ('smart' meaning that it updates itself every few seconds)
	 * The list is sorted by the RSSI (strength of signal)
	 * TODO: we only returning the first 7 networks for now (handle this better at controller side)
	 * */
	std::vector<sm::WiFiNetwork>& getNearbyNetworks() {
		std::vector<sm::WiFiNetwork>& networks = nearbyNetworksCache.get();

		std::sort(networks.begin(), networks.end());
		//std::reverse(networks.begin(), networks.end());
		networks.erase(networks.begin(), networks.end() - 7);
		return networks;
	}


	/*
	 * Networks you can choose from to connect
	 * */
	std::vector<sm::WiFiNetwork>& scan(){
		return nearbyNetworksCache.forceUpdate();
	}

	/*
	 * try to connect to a network; toSave- do you want to save that network? (maybe for auto connect or just the password)
	 * */
	void connect(sm::WiFiNetwork wifiNet, bool toSave) {
	//	Serial.println("^^^^^^^^^^^^^^^^^^ Inside station.connect (trying to connect to wifiNetwork)");
//		if(scheduler == nullptr)
	//		Serial.println("________ERROR________^^^^^^^^^^^^^^^^^^ scheduler is null in Station");
		//disconnect();
		auto lambdatryToWaitForConnection = [=](){return this->updateConnectionStatusAndMaybeSaveNetwork(wifiNet, toSave);};
		tsk = scheduler->addTaskWithInterval(std::chrono::seconds{10}, TASK_FOREVER, lambdatryToWaitForConnection);//check every sec if we connected
	}

	/*
	 * Disconnect from the current network (if connect to one)
	 * */
	bool disconnect() {
		tsk = nullptr;// if we have a task which wating for connection
		bool ret = WiFi.disconnect();
		//WiFi.mode(WIFI_OFF);
		//WiFi.mode(WIFI_AP_STA);
		if(ret)
			connectionStatus.init();
	//	else
	//		Serial.println("___ERROR__: failed disconnecting the wifi");
		return ret;
	}

	/*
	 * Checks if one of the saved networks with connect flag is on is available and connects to it
	 * */
	void connectAuto() {
		std::vector<sm::WiFiNetwork>& nearbyNetworks = scan();
		for(auto& savedNetwork : savedNetworks){
			if(savedNetwork.autoConnect){
				auto p = find(nearbyNetworks.begin(), nearbyNetworks.end(), savedNetwork);
				if(p != nearbyNetworks.end())// one of the saved networks is available
					connect(savedNetwork, false);
			}
		}
	}

	/*
	 * Deletes a network from the saved networks
	 * */
	void forgetNetwork(WiFiNetwork& networkToForget) {
		auto p = find(savedNetworks.begin(), savedNetworks.end(), networkToForget);
		if(p != savedNetworks.end())// one of the saved networks is available
			savedNetworks.erase(p);
	}

	void forgetNetwork(String SSID) {
		auto SSIDComperator = [&](WiFiNetwork network){ return network.SSID == SSID;};
		auto p = std::find_if(savedNetworks.begin(), savedNetworks.end(), SSIDComperator);
		if(p != savedNetworks.end())// one of the saved networks is available
			savedNetworks.erase(p);
	}


	std::vector<sm::WiFiNetwork>& getSavedNetworks() {
		return savedNetworks;
	}

	std::shared_ptr<void> accept(IStationVisitor& visitor) {
		return visitor.visit(*this);
	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const {
		archive.addProperties(mappingFile, MACRO_NVP(savedNetworks), MACRO_NVP(connectionStatus));
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
		archive.loadProperties(mappingFile, MACRO_NVP(savedNetworks), MACRO_NVP(connectionStatus));
	}


	void saveNetwork(sm::WiFiNetwork& wifiNet){
		ptrdiff_t pos = std::distance(savedNetworks.begin(), find(savedNetworks.begin(), savedNetworks.end(), wifiNet));
		if(pos >= savedNetworks.size())//not saved yet - so save it
			savedNetworks.push_back(wifiNet);
		else //if already exists update it
			savedNetworks[pos] = wifiNet;
	}
protected:
	/*
	 * Networks you can choose from to connect
	 * */
	 void scanInner(std::vector<sm::WiFiNetwork>& networksOut){
	//	Serial.println("111111111111111111Inside scanInner");
	//	std::vector<sm::WiFiNetwork> networks;
		int n = WiFi.scanNetworks();
	//	Serial.println("scan done");
		for (int i = 0; i < n; ++i){
			String ssid = WiFi.SSID(i);
			int rssi = WiFi.RSSI(i);
			String bssid = WiFi.BSSIDstr(i);
			int ch = WiFi.channel(i);

			networksOut.push_back(sm::WiFiNetwork(ssid, rssi, bssid, ch, static_cast<wl_enc_type>(WiFi.encryptionType(i))));
		}
	//	Serial.println("111111111111111111 exiting scanInner");
	//	return networks;
	}

	void updateConnectionStatus() const {
		connectionStatus.isConnected = WiFi.status() == WL_CONNECTED;
		connectionStatus.network.BSSID = WiFi.BSSIDstr();
		connectionStatus.network.RSSI = WiFi.RSSI();
		connectionStatus.network.SSID = WiFi.SSID();
		connectionStatus.network.channel = WiFi.channel();
		//connectionStatus.network.encryptionType = WiFi.encryptionType();

		connectionStatus.localIp = WiFi.localIP().toString();
	}

	void updateConnectionStatusAndMaybeSaveNetwork(sm::WiFiNetwork wifiNet, bool toSave) {
		/*Serial.println("^^^^^^^^^^^^^^^^^^^^^^^^^^^ Inside updateConnectionStatusAndMaybeSaveNetwork");
		if(WiFi.status() != WL_CONNECTED ){//while we not connected
			Serial.print(WiFi.status());
			Serial.print("trying to connect to: ");Serial.println(wifiNet.SSID);
			Serial.print("pass: ");Serial.println(wifiNet.password);
			connectInner(wifiNet);
			return;
		}*/
		//we connected
		tsk = nullptr;//end this task
		if(toSave)//save the network if we specified it
			saveNetwork(wifiNet);
		//update that we connected to this network
		connectionStatus.network = wifiNet;
		updateConnectionStatus();

		//print stuff
//		Serial.println ( "" );
//		Serial.print ( "^^^^^^^^^^^^^^^^^^^^^^^^^^^^Connected to " );
//		Serial.println ( connectionStatus.network.SSID );
//		Serial.print ( "^^^^^^^^^^^^^^^^^^^^^^^^^^^^IP address: " );
//		Serial.println ( WiFi.localIP() );
	}

	void connectInner(sm::WiFiNetwork& wifiNet){
		WiFi.begin(wifiNet.SSID.c_str(), wifiNet.password.c_str());
	}

};

} /* namespace sm */

#endif /* STATIONMODULE_MODEL_STATION_H_ */
