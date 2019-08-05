/*
 * SchedulerModule.h
 *
 *  Created on: 6 במרץ 2018
 *      Author: rina
 */

#ifndef SCHEDULERMODULE_SCHEDULERMODULE_H_
#define SCHEDULERMODULE_SCHEDULERMODULE_H_

#include <vector>
#include <memory>
#include <ModuleFramework/Module.h>
#include <ModuleFramework/Container/Container.h>

#include <SchedulerModule/service/SchedulerService.h>

namespace schedmodule {

std::shared_ptr<sched::SchedulerService> schedulerServiceCreator(std::shared_ptr<tsm::TimeService> timeService) {
	return std::make_shared<sched::SchedulerService>(timeService);
}

class SchedulerModule : public MF::ModuleBase {
public:
	SchedulerModule() {}
	~SchedulerModule() {}

	void start(std::shared_ptr<cntnr::Container> container){
	//	Serial.println("Inside SchedulerModule start ");
		container->registerType<sched::SchedulerService>(&schedulerServiceCreator);
	//	Serial.println("End Inside SchedulerModule start ");
	}

};

} /* namespace schedmodule */

#endif /* SCHEDULERMODULE_SCHEDULERMODULE_H_ */
