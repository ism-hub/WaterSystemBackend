/*
 * PlantsSuiteController.h
 *
 *  Created on: 6 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_CONTROLLERS_PLANTSSUITECONTROLLER_H_
#define HARDWAREMODULE_CONTROLLERS_PLANTSSUITECONTROLLER_H_

#include <memory>
#include <vector>
#include <algorithm>
#include <functional>

#include <SchedulerService.h>
#include <GardenUnitOfWork.h>

namespace hrdwrctrl {

class PlantsSuiteController {

	// according to the time-pattern in the program (for specific plant) change the pump and sprinkler status to on/off
	// (only changing the status in the model, not actually talking with the hardware,
	//  the sprinkler controller and the pump controller will in turn see the change and talk to the hardware)
	class PlantSuiteConroller {
		int programTimePatternB4HookId;
		int programTimePatternHookId;

		int plantSprinklerHookId;
		int plantSprinklerB4HookId;

		int plantProgramB4HookId;
		int plantProgramHookId;


		std::vector<std::shared_ptr<Task>> shceduledTaskIds;//to remove the program execution we need to remove all those tasks from the scheduler

		std::shared_ptr<sched::SchedulerService>  _scheduleService;

	public:
		std::weak_ptr<GardenModel::Plant> _plant;

		PlantSuiteConroller(std::weak_ptr<GardenModel::Plant> plant, std::shared_ptr<sched::SchedulerService> scheduleService) : _scheduleService(scheduleService), _plant(plant) {
			addHooks();
			//add tasks to the scheduler
			executePlantProgram();
		}

		PlantSuiteConroller(const PlantSuiteConroller& plantSuiteController) {
			_plant = plantSuiteController._plant;
			_scheduleService = plantSuiteController._scheduleService;
			addHooks();
			executePlantProgram();
		}

		PlantSuiteConroller& operator=(const PlantSuiteConroller& plantSuiteController) {
			stopPlantProgramExecution();
			removeHooks();
			_plant = plantSuiteController._plant;
			_scheduleService = plantSuiteController._scheduleService;
			addHooks();
			executePlantProgram();
			return *this;
		}

		PlantSuiteConroller(PlantSuiteConroller&& plantSuiteController) {
			_plant = plantSuiteController._plant;
			_scheduleService = plantSuiteController._scheduleService;
			addHooks();
			executePlantProgram();

			//plantSuiteController.stopPlantProgramExecution();
			//plantSuiteController.removeHooks();
			//plantSuiteController._plant = {};
		}

		void addProgramTimepatternHooks(){
			//timepattern hook
			if(_plant.lock()->_program.get() != nullptr) {
				auto lambdaB4ProgramTimePatternChange = [=](GardenModel::TimePattern timePattern){return this->beforeProgramTimePatternChange(timePattern);};
				programTimePatternB4HookId = _plant.lock()->_program.get()->timePattern.before_change().connect(lambdaB4ProgramTimePatternChange);
				auto lambdaOnProgramTimePatternChange = [=](GardenModel::TimePattern timePattern){return this->onProgramTimePatternChange(timePattern);};
				programTimePatternHookId = _plant.lock()->_program.get()->timePattern.on_change().connect(lambdaOnProgramTimePatternChange);
			}
		}

		void addHooks() {
			Serial.print("plantSuite addHooks");Serial.println(_plant.lock()->id);
			//program hooks
			auto lambdaBeforeProgramChange = [=](std::shared_ptr<GardenModel::SimpleProgram> program){return this->beforProgramChange(program);};
			plantProgramB4HookId = _plant.lock()->_program.before_change().connect(lambdaBeforeProgramChange);
			auto lambdaOnProgramChange = [=](std::shared_ptr<GardenModel::SimpleProgram> program){return this->onProgramChange(program);};
			plantProgramHookId = _plant.lock()->_program.on_change().connect(lambdaOnProgramChange);

			addProgramTimepatternHooks();

			//sprinkler hook
			auto lambdaB4PlantSprinklerChange = [=](std::shared_ptr<GardenModel::Sprinkler> sprinkler){return this->beforePlantSprinklerChange(sprinkler);};
			plantSprinklerHookId = _plant.lock()->_sprinkler.before_change().connect(lambdaB4PlantSprinklerChange);
			auto lambdaOnPlantSprinklerChange = [=](std::shared_ptr<GardenModel::Sprinkler> sprinkler){return this->onPlantSprinklerChange(sprinkler);};
			plantSprinklerHookId = _plant.lock()->_sprinkler.on_change().connect(lambdaOnPlantSprinklerChange);
		}

		void removeProgramTimePatternHooks(){
			if(_plant.lock()->_program.get() != nullptr ) {
				_plant.lock()->_program.get()->timePattern.before_change().disconnect(programTimePatternB4HookId);
				_plant.lock()->_program.get()->timePattern.on_change().disconnect(programTimePatternHookId);
			}
			programTimePatternB4HookId = -1;
			programTimePatternHookId = -1;
		}

		void removeHooks() {
			Serial.print("plantSuite removeHooks ");Serial.println(_plant.lock()->id);
			if(_plant.expired())
				return;
			_plant.lock()->_program.before_change().disconnect(plantProgramB4HookId);
			_plant.lock()->_program.on_change().disconnect(plantProgramHookId);

			removeProgramTimePatternHooks();

			_plant.lock()->_sprinkler.before_change().disconnect(plantSprinklerB4HookId);
			_plant.lock()->_sprinkler.on_change().disconnect(plantSprinklerHookId);
			plantProgramB4HookId = -1;
			plantProgramHookId = -1;
			plantSprinklerB4HookId = -1;
			plantSprinklerHookId = -1;
		}

		~PlantSuiteConroller() {
			removeHooks();
			stopPlantProgramExecution();
		}

		void executePlantProgram() {
			Serial.print("plantSuite executePlantProgram");Serial.println(_plant.lock()->id);
			if (_plant.lock()->_program == nullptr || _plant.lock()->_sprinkler == nullptr ) {
				Serial.println("plantSuite executePlantProgram don't have sprinkler or program so we won't execute");
				return;
			}
			auto& days = _plant.lock()->_program.get()->timePattern.get().days;
			for(auto& day : days){
				for(auto& hour : day.hours){
					auto lambdaStartWaterPlant = [=](){return this->startWaterPlant();};
					std::shared_ptr<Task> taskStartWater = _scheduleService->addTaskAtHour(hour, days.size(), lambdaStartWaterPlant);// @param- frequency - 1 every day at that hour, 2 - every 2 days at that hour
					GardenModel::Hour hourPlusFewSec = hour;

					auto hourTimePoint = myDate::my_clock::time_point{std::chrono::seconds{hour.hour*60*60 + hour.min*60 + hour.sec}};
					hourTimePoint += std::chrono::seconds{hour.duration};
					auto currentDays = std::chrono::time_point_cast<myDate::days>(hourTimePoint);
					auto secFromDayStart = hourTimePoint - currentDays;
					auto timeToday = myDate::make_time(secFromDayStart);

					hourPlusFewSec.hour = timeToday.hours().count();
					hourPlusFewSec.min = timeToday.minutes().count();
					hourPlusFewSec.sec = timeToday.seconds().count();

					auto lambdaStoptWaterPlant = [=](){return this->stopWaterPlant();};
					std::shared_ptr<Task> taskStopWater = _scheduleService->addTaskAtHour(hourPlusFewSec, days.size(), lambdaStoptWaterPlant); // after 15 sec stop the watering
					shceduledTaskIds.push_back(taskStartWater);
					shceduledTaskIds.push_back(taskStopWater);
				}
			}
		}

		void stopPlantProgramExecution() {
			Serial.print("plantSuite stopPlantProgramExecution");Serial.println(_plant.lock()->id);
			if(shceduledTaskIds.size() == 0){
				Serial.println("   We didn't execute it so we have nothing to stop.");
				return;
			}
			for(auto& id : shceduledTaskIds)
				_scheduleService->removeTask(id);
			shceduledTaskIds.clear();
			stopWaterPlant();
		}

		void beforProgramChange(std::shared_ptr<GardenModel::SimpleProgram> newProgram) {
			Serial.print("plantSuite beforeProgramChange, plant id - ");Serial.print(_plant.lock()->id);
			Serial.print(" from program id - "); _plant.lock()->_program.get() != nullptr ? Serial.print(_plant.lock()->_program.get()->id) : Serial.print("null");
			Serial.print(" to program id - ");newProgram != nullptr ? Serial.println(newProgram->id) : Serial.println("null");
			stopPlantProgramExecution();//stops the old one
			removeProgramTimePatternHooks();
		}

		void onProgramChange(std::shared_ptr<GardenModel::SimpleProgram> newProg) {
			addProgramTimepatternHooks();
			executePlantProgram();//runs the new one
		}

		void beforeProgramTimePatternChange(GardenModel::TimePattern newTimePattern) {// make sure that the plant still have that program
			Serial.print("plantSuite beforeProgramTimePatternChange, plant id - ");Serial.print(_plant.lock()->id);
			Serial.print(" program id - ");Serial.println(_plant.lock()->_program.get()->id);
			stopPlantProgramExecution();
		}

		void onProgramTimePatternChange(GardenModel::TimePattern newTimePattern) {// make sure that the plant still have that program
			Serial.print("plantSuite onProgramTimePatternChange, plant id - ");Serial.print(_plant.lock()->id);
			Serial.print(" program id - ");Serial.println(_plant.lock()->_program.get()->id);
			executePlantProgram();
		}

		void beforePlantSprinklerChange(std::shared_ptr<GardenModel::Sprinkler> newSprinkler) {
			Serial.print("plantSuite beforePlantSprinklerChange, plant id - ");Serial.print(_plant.lock()->id);
			Serial.print(" sprinkler id - ");
			_plant.lock()->_sprinkler.get() != nullptr ? Serial.print(_plant.lock()->_sprinkler.get()->id) : Serial.print("null");
			Serial.print(" new sprinkler is - "); newSprinkler != nullptr ? Serial.println(newSprinkler->id) : Serial.println("null");;
			stopPlantProgramExecution();
		}

		void onPlantSprinklerChange(std::shared_ptr<GardenModel::Sprinkler> newSprinkler) {
			Serial.print("plantSuite onPlantSprinklerChange, plant id - ");Serial.print(_plant.lock()->id);
			Serial.print(" Sprinkler id - ");_plant.lock()->_sprinkler.get() != nullptr ? Serial.println(_plant.lock()->_sprinkler.get()->id) : Serial.println("null");
			executePlantProgram();
		}

		void startWaterPlant() {
			Serial.print("plantSuite startWaterPlant ");Serial.println(_plant.lock()->id);
			_plant.lock()->_sprinkler.get()->status = GardenModel::Sprinkler::Status::On;
			_plant.lock()->garden->_pump->status = GardenModel::Pump::On;
		}

		void stopWaterPlant() {
			Serial.print("plantSuite stopWaterPlant ");Serial.println(_plant.lock()->id);
			if(_plant.lock()->garden->_pump != nullptr)
				if(isAllSprinklersWillBeClose(_plant.lock()->_sprinkler.get()->id))// if all sprinklers are close and the pump is working the water dont have a  way put, we need to close the pump first and than close the last sprinkler
					_plant.lock()->garden->_pump->status = GardenModel::Pump::Off;
			if (_plant.lock()->_sprinkler.get() != nullptr) {
				_plant.lock()->_sprinkler.get()->status = GardenModel::Sprinkler::Status::Off;
			}
		}

		//@param[in] - id - the sprinkler we want to close (we check if all the others are close)
		bool isAllSprinklersWillBeClose(int id) {
			Serial.println("inside isAllSprinklersWillBeClose");
			for(auto& sprinkler : _plant.lock()->garden->_sprinklers.getInnerVector()){
				if((sprinkler.get()->id != id) &&
					(sprinkler.get()->status == GardenModel::Sprinkler::Status::On)){
						Serial.println("ret false - there is an sprinkler (besides us) which open now");
						return false;
				}
			}
			Serial.println("ret true - we are the only sprinkler which open");
			return true;
		}
	};

	std::vector<std::shared_ptr<PlantSuiteConroller>> _controllers;
	std::shared_ptr<sched::SchedulerService> _scheduleService;
	std::shared_ptr<DAL::GardenUnitOfWork> _unitOfWork;
	int hookId;// the id of the function we sent to the signal

public:
	PlantsSuiteController(std::shared_ptr<sched::SchedulerService> scheduleService, std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork)
		: _scheduleService(scheduleService), _unitOfWork(unitOfWork)
	{
		// tracking after the plants, will update the scheduler now based on the plant program
		for(auto& plant : _unitOfWork->Plants().getAll()) {
			auto plantSuiteController = std::make_shared<PlantSuiteConroller>(plant, _scheduleService);
			_controllers.push_back(plantSuiteController);// check if this line do what you think it do
		}
		auto lambdaOnPlantsChange = [=](Model::Property<std::shared_ptr<GardenModel::Plant>> plant, Model::Change change){return this->onPlantsChange(plant, change);};
		hookId = _unitOfWork->Garden().getById(-1)->_plants.on_change().connect(lambdaOnPlantsChange);

	}

	virtual ~PlantsSuiteController() {
		Serial.print("Number of controllers: ");Serial.println((int)(_controllers.size()));
		Serial.print("Number of plant suites: ");Serial.println((int)(_unitOfWork->Garden().getById(-1)->_plants.size()));
		_unitOfWork->Garden().getById(-1)->_plants.on_change().disconnect(hookId);
	}

protected:

	void onPlantsChange(Model::Property<std::shared_ptr<GardenModel::Plant>> plant, Model::Change change) {
		Serial.print("Called onPlantsChange ");Serial.println(plant.get()->id);
		if(change == Model::Change::Added)
			onNewPlant(plant);
		else if(change == Model::Change::Deleted)
			onPlantDelete(plant);
	}

	void onNewPlant(Model::Property<std::shared_ptr<GardenModel::Plant>> plant) {
		Serial.print("Creating controller for new plant ");Serial.println(plant.get()->id);
		auto plantSuiteCtrl = std::make_shared<PlantSuiteConroller>(plant.get(), _scheduleService);
		_controllers.push_back(plantSuiteCtrl);
	}

	void onPlantDelete(Model::Property<std::shared_ptr<GardenModel::Plant>> plant) {
		auto comperator = [&](std::shared_ptr<PlantSuiteConroller> plantSuiteController){ return plantSuiteController->_plant.lock()->id == plant.get()->id;};
		auto position = std::find_if(_controllers.begin(), _controllers.end(), comperator);
		if (position != _controllers.end()){ // == myVector.end() means the element was not found
			_controllers.erase(position);
			Serial.print("Deleted plant ");Serial.println(plant.get()->id);
		}else{
			Serial.print("ERROR? we on plant delete but we didnt deleted anything, plant id - ");Serial.println(plant.get()->id);
		}
	}

};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_CONTROLLERS_PLANTSSUITECONTROLLER_H_ */
