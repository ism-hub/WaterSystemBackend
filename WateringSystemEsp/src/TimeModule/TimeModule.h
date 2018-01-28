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

namespace TM {

std::shared_ptr<TS::TimeService> timeServiceCreator(){
	return std::make_shared<TS::TimeService>();
}


class TimeModule : public MF::ModuleBase{
public:
	TimeModule(){

	}
	virtual ~TimeModule(){

	}

	void start(std::shared_ptr<cntnr::Container> container){
		container->registerType<TS::TimeService>(&timeServiceCreator);
	}
};

} /* namespace TM */

#endif /* TIMEMODULE_TIMEMODULE_H_ */
