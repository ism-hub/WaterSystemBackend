/*
 * TaskScheduler.h
 *
 *  Created on: 12 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWARETESTS_TASKSCHEDULER_SRC_TASKSCHEDULER_H_
#define HARDWARETESTS_TASKSCHEDULER_SRC_TASKSCHEDULER_H_

#include <chrono>
#include <thread>

//#define TASK_MILLISECOND       1UL
#define TASK_SECOND         1UL
#define TASK_MINUTE        60UL
#define TASK_HOUR        3600UL


#define TASK_IMMEDIATE          0
#define TASK_FOREVER         (-1)
#define TASK_ONCE               1

//moking the scheduler library im using in the esp with this simple scheduler which uses delays and threads
class Task {
public:
	//static int idCounter;
	unsigned long interval;
	long iterations;
	std::function<void(void)> callback;

	bool enabled = false;
	int id_ = 0;// = ++idCounter;

	Task(unsigned long aInterval=0, long aIterations=0, std::function<void(void)> aCallback=nullptr) {
		//id = idCounter;
		//idCounter += 1;
		std::cout << "task CTOR" << std::endl;
		interval = aInterval;
		iterations = aIterations;
		callback = aCallback;
	}

	virtual ~Task() {
		std::cout << "DTOR of taks id -" << std::endl;
		std::cout << id_ << std::endl;
	}

	void enableDelayed(unsigned long aDelay = 0) {
		setDelay(aDelay);
		enabled = true;
	}

	void setDelay(unsigned long delay){
		this->delay = delay;
		lastTimeWeSetDelay = time(0);
	}

	unsigned long getDelay(){
		return delay;
	}

	unsigned long getLastTimeWeSetDelay(){
		return lastTimeWeSetDelay;
	}

private:
	unsigned long delay = 0;
	unsigned long lastTimeWeSetDelay = 0;
};


class Scheduler {
public:
	std::vector<Task*> tasks;
	int idCounter = 0;
	//unsigned long startTime = time(0);

	Scheduler(){}

	virtual ~Scheduler(){}

	void addTask(Task& tsk) {
		tsk.id_ = ++idCounter;
		std::cout << "Added task with id #################33333333333333 - "<< std::endl;
		std::cout << tsk.id_ << std::endl;
		tasks.push_back(&tsk);
	}

	void deleteTask(Task& tsk) {
		std::cout << "Deleting task - " << std::endl;
		std::cout << tsk.id_ << std::endl;
		std::cout << "Before delete we have " << std::endl;
		std::cout << tasks.size() << std::endl;
		std::cout <<  "tasks" << std::endl;
		tasks.erase(std::find_if(tasks.begin(), tasks.end(), [&](const Task* oTsk){return oTsk->id_ == tsk.id_;}));
		std::cout << "After delete we have " << std::endl;
		std::cout << tasks.size() << std::endl;
		std::cout << "tasks" << std::endl;
	}

	void executeImmediateOnce(){
		for(auto tsk : tasks) {
			std::cout << "exe task ";
			std::cout << tsk->id_ << std::endl;
			tsk->callback();
		}
	}

	void execute(){
		//executeImmediateOnce();
		//return;
		//here we just poll the tasks and if its his time we run it
		while(true)
		{
			std::this_thread::sleep_for(std::chrono::seconds(10));
			unsigned long timeNow = time(0);
			std::cout << " number of tasks we have " << std::endl;
			std::cout << tasks.size() << std::endl;
			for(auto tsk : tasks) {
				std::cout << "time already passed for the task " << std::endl;
				unsigned long delta = timeNow - tsk->getLastTimeWeSetDelay();
				std::cout << delta << std::endl;
				std::cout << " tsk->getDelay() " << std::endl;
				std::cout <<  tsk->getDelay() << std::endl;
				long deltaLeft = tsk->getDelay() - delta;
				std::cout << " time left till run " << std::endl;
				std::cout << deltaLeft << std::endl;
				if(tsk->enabled && (deltaLeft <= 0)){
					std::cout << "Need to exe task" << std::endl;
					tsk->callback();
					tsk->setDelay(tsk->interval);
				}
			}
		}
	}
};


#endif /* HARDWARETESTS_TASKSCHEDULER_SRC_TASKSCHEDULER_H_ */
