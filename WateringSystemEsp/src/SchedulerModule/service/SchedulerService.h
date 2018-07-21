/*
 * SchedulerService.h
 *
 *  Created on: 6 במרץ 2018
 *      Author: rina
 */

#ifndef SCHEDULERMODULE_SERVICE_SCHEDULERSERVICE_H_
#define SCHEDULERMODULE_SERVICE_SCHEDULERSERVICE_H_

#include <TimePattern.h>
#include <functional>

//************ task schedule stuff
#define _TASK_WDT_IDS
#define _TASK_STD_FUNCTION
#include <TaskScheduler/src/TaskScheduler.h>
#include <TimeService.h>

namespace sched {

//The API - I will decide on the interface while building the thing which uses it,
//	suppose to be straight forward (just calling the the scheduler lib for most parts)
//	(so no much of a complex logic here)
class SchedulerService {
	Scheduler ts;
	std::shared_ptr<TS::TimeService> _timeService;
public:
	SchedulerService(std::shared_ptr<TS::TimeService> timeService) : _timeService(timeService) {

	}
	virtual ~SchedulerService() {}

	std::shared_ptr<Task> addTaskAtHour(GardenModel::Hour hour, int frequency, std::function<void()> taskFnc){
		std::shared_ptr<Task> tsk = std::make_shared<Task>(TASK_HOUR*24*frequency, TASK_FOREVER, taskFnc);
		ts.addTask(*tsk);
		auto currTime = _timeService->getCurrentDateTime();
		GardenModel::Hour hourNow;
		hourNow.hour = currTime.hours().count();
		hourNow.min = currTime.minutes().count();
		hourNow.sec = currTime.seconds().count();

		unsigned long int delayInSec = hour - hourNow > 0 ? hour - hourNow : 24*60*60 + (hour - hourNow);
		Serial.print("    Delay in sec - ");Serial.println(delayInSec);
		tsk->enableDelayed(delayInSec*TASK_SECOND);
		return tsk;
	}

	void removeTask(std::shared_ptr<Task> task) {
		Serial.print("Deleting task with id - ");//Serial.println(task->id_);
		ts.deleteTask(*task);
	}

	void execute() {
		ts.execute();
	}

};

} /* namespace sched */

#endif /* SCHEDULERMODULE_SERVICE_SCHEDULERSERVICE_H_ */
