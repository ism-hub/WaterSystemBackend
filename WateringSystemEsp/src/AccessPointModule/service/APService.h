/*
 * APService.h
 *
 *  Created on: 27 αιεμι 2018
 *      Author: IsM
 */

#ifndef ACCESSPOINTMODULE_SERVICE_APSERVICE_H_
#define ACCESSPOINTMODULE_SERVICE_APSERVICE_H_

#include <IService.h>
#include <memory>
#include <SchedulerService.h>

#include <AccessPointModule/configuration/model/APConfiguration.h>
#include <AccessPointModule/status/model/APStatus.h>

namespace apm {

//starts the the AccessPoint or stop it
class APService : public sfwk::IService {
	std::shared_ptr<sched::SchedulerService> scheduler;
	std::shared_ptr<Task> tsk = nullptr;// the task we added to the service
	APConfiguration apConf;
	APStatus apStatus;
public:
	APService(std::shared_ptr<sched::SchedulerService> scheduler, APConfiguration apConf) :
		sfwk::IService("APService"), scheduler(scheduler), apConf(apConf), apStatus(apStatus) {}

	virtual ~APService(){}

	//creates a tast that try to get it up every sec till succeed
	int StartService() override {
		WiFi.softAPdisconnect();
		apStatus.init();
		Serial.println("Inside APService::StartService");
		auto lambdatryToCreateAP = [=](){return this->tryToCreate();};
		tsk = scheduler->addTaskWithInterval(std::chrono::seconds{1}, TASK_FOREVER, lambdatryToCreateAP);
		return 0;
	}

	//close the AP
	int StopService() override {
		tsk = nullptr;
		WiFi.softAPdisconnect();
		apStatus.init();
		return 0;
	}

	APStatus& getAPStatus() {
		updateAPStatus();
		return apStatus;
	}

	void updateAPStatus() {
		apStatus.ipAddress = WiFi.softAPIP().toString();
		apStatus.macAddress = WiFi.softAPmacAddress();
		apStatus.numOfConnectedStations = WiFi.softAPgetStationNum();
	}

	APConfiguration getAPConfiguration() {
		return apConf;
	}

	void setAPConfiguration(APConfiguration apConf) {
		StopService();
		this->apConf = apConf;
		StartService();
	}

protected:
	//tries to create the access point
	void tryToCreate() {//TODO: handle errors
		Serial.println("Trying to create AP ***************");
		IPAddress localIp;
		localIp.fromString(apConf.localIP);
		Serial.println(localIp.toString());
		IPAddress gateway;
		gateway.fromString(apConf.gateway);
		Serial.println(gateway.toString());
		IPAddress subnet;
		subnet.fromString(apConf.subnet);
		Serial.println(subnet.toString());

		WiFi.softAPConfig(localIp, gateway, subnet);
		Serial.println(apConf.ssid.c_str());
		Serial.println(apConf.password.c_str());
		boolean result = WiFi.softAP(apConf.ssid.c_str(), apConf.password.c_str());
		if(result == true){
			tsk = nullptr;//the DTOR of this makes sure it kills the task from the SchedulerService
			apStatus.isOn = true;
			Serial.println("Succeeded to create AP");
		}else{
			apStatus.isOn = false;
			Serial.println("Failed to create AP (will try again in 1 sec)");
		}
	}

};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_SERVICE_APSERVICE_H_ */
