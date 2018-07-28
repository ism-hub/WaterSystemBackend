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

#include <AccessPointModule/model/APConfiguration.h>

namespace apm {

//starts the the AccessPoint or stop it
class APService : public sfwk::IService {
	std::shared_ptr<sched::SchedulerService> scheduler;
	std::shared_ptr<Task> tsk = nullptr;// the task we added to the service
	APConfiguration apConf;
public:
	APService(std::shared_ptr<sched::SchedulerService> scheduler, APConfiguration apConf) :
		sfwk::IService("APService"), scheduler(scheduler), apConf(apConf) {}

	virtual ~APService(){}

	//creates a tast that try to get it up every sec till succeed
	int StartService() override {
		Serial.println("Inside APService::StartService");
		auto lambdatryToCreateAP = [=](){return this->tryToCreate();};
		tsk = scheduler->addTaskWithInterval(std::chrono::seconds{1}, TASK_FOREVER, lambdatryToCreateAP);
		return 0;
	}

	//close the AP
	int StopService() override {
		tsk = nullptr;
		WiFi.softAPdisconnect();
		return 0;
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
	void tryToCreate() {
		Serial.println("Trying to create AP ***************");
		IPAddress localIp(192,168,69,1);
		IPAddress gateway(192,168,11,255);
		IPAddress subnet(255,255,255,0);
		WiFi.softAPConfig(localIp, gateway, subnet);
		Serial.println(apConf.ssid.c_str());
		Serial.println(apConf.password.c_str());
		boolean result = WiFi.softAP(apConf.ssid.c_str(), apConf.password.c_str());
		if(result == true){
			tsk = nullptr;//the DTOR of this makes sure it kills the task from the SchedulerService
			Serial.println("Succeeded to create AP");
		}else{
			Serial.println("Failed to create AP (will try again in 1 sec)");
		}
	}

};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_SERVICE_APSERVICE_H_ */
