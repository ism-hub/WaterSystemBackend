/*
 * TimeModule.h
 *
 *  Created on: 11 срхїз 2017
 *      Author: IsM
 */

#ifndef TIMEMODULE_TIMEMODULE_H_
#define TIMEMODULE_TIMEMODULE_H_

#include <memory>
#include <ModuleFramework/Module.h>
#include <ModuleFramework/Container/Container.h>

#include <TimeModule/timeService/TimeService.h>
#include <TimeModule/DAL/NtpSettingsContex.h>

#include <DALModule/DALModule.h>

namespace tsm {

std::shared_ptr<TimeService> timeServiceCreator(std::shared_ptr<NtpSettingsContex> ntpSettingsContex){
//	if(ntpSettingsContex == nullptr)
//		Serial.println("______ERROR_______: timeServiceCreator ntpSettingsContex == nullptr");
	return std::make_shared<TimeService>(ntpSettingsContex);
}

std::shared_ptr<NtpSettingsContex> ntpSettingsContexCreator(std::shared_ptr<DALModule::DefaultSerializationService> serService){
//	if(serService == nullptr)
//		Serial.println("______ERROR_______: ntpSettingsContexCreator serService == nullptr");
	return std::make_shared<NtpSettingsContex>(serService, "ntpSettings.json");
}


class TimeModule : public MF::ModuleBase{
public:
	TimeModule(){

	}
	~TimeModule(){

	}

	void start(std::shared_ptr<cntnr::Container> container){
	//	Serial.println("Inside TimeModule start ");
		container->registerType<TimeService>(&timeServiceCreator);
		container->registerType<NtpSettingsContex>(&ntpSettingsContexCreator);

		//starting the server
		std::shared_ptr<TimeService> timeService = container->resolve<TimeService>();
		std::shared_ptr<sfwk::ServiceFrameWork> serviceFramework = container->resolve<sfwk::ServiceFrameWork>();
		serviceFramework->AddService(timeService);
		timeService->StartService();
//		Serial.println("END Inside TimeModule start ");
	}
};

} /* namespace TM */

#endif /* TIMEMODULE_TIMEMODULE_H_ */
