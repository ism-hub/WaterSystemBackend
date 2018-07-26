/*
 * SprinklerController.h
 *
 *  Created on: 6 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_CONTROLLERS_SPRINKLERSCONTROLLER_H_
#define HARDWAREMODULE_CONTROLLERS_SPRINKLERSCONTROLLER_H_

#include <memory>
#include <GardenUnitOfWork.h>

#include <SwitchArray.h>

namespace hrdwrctrl {

class SprinklersController {

	class SprinklerController {
	public:
		std::weak_ptr<GardenModel::Sprinkler> _sprinkler = {};
		int hookOnStatusChangeId;
		std::shared_ptr<hrdwrctrl::SwitchArray> _switchHub = nullptr;

		SprinklerController(std::weak_ptr<GardenModel::Sprinkler> sprinkler, std::shared_ptr<hrdwrctrl::SwitchArray> switchHub) : _sprinkler(sprinkler), _switchHub(switchHub) {
			Serial.print("CTOR sprinkler controller, for sprinkler id - ");Serial.println(sprinkler.lock()->id.get());
			addHooks();
		}

		SprinklerController(const SprinklerController& sprinklerController) {
			_sprinkler = sprinklerController._sprinkler;
			_switchHub = sprinklerController._switchHub;
			addHooks();
		}


		SprinklerController& operator=(const SprinklerController& sprinklerController) {
			removeHooksToEvents();
			_sprinkler = sprinklerController._sprinkler;
			_switchHub = sprinklerController._switchHub;
			//hookOnStatusChangeId = sprinklerController.hookOnStatusChangeId;
			addHooks();
			return *this;
		}

		SprinklerController(SprinklerController&& sprinklerController) {
			_sprinkler = sprinklerController._sprinkler;
			_switchHub = sprinklerController._switchHub;
			sprinklerController.removeHooksToEvents();
			addHooks();
			sprinklerController._sprinkler = {};
		}

		~SprinklerController() {

			if(!_sprinkler.expired()){
				Serial.print("CTOR sprinkler controller, for sprinkler id - ");Serial.println(_sprinkler.lock()->id.get());
				removeHooksToEvents();
			}else
				Serial.println("_sprinkler is expired");
			//closeSprinkler - do we want to close the sprinkler if he is open? is make any sense that we will delete controller on an open sprinkler?
		}

		void removeHooksToEvents() {
			_sprinkler.lock()->status.on_change().disconnect(hookOnStatusChangeId);
			hookOnStatusChangeId = -1;
		}

		void addHooks() {
			auto lambdaOnStatuschange = [=](Sprinkler::Status status){return this->onStatusChange(status);};
			hookOnStatusChangeId = _sprinkler.lock()->status.on_change().connect(lambdaOnStatuschange);
		}

		//TODO: "dryRun"??? get rid of that problem
		bool dryRun = false;//on fail when we want to revert back we set it to false
		void onStatusChange(Sprinkler::Status status) {
			Serial.println("@@@@@@@@@@ Changing sprinkler status @@@@@@@@@@");
			if(dryRun){
				Serial.println("__doing dryRun to SprinklersConroller onStatusChange.");
				dryRun = false;
				return;
			}
			//digitalWrite(getSprinklerPin(), status == Sprinkler::Status::On ? HIGH : LOW);

			Serial.print("Sprinkler ");Serial.print(_sprinkler.lock()->id);Serial.print(" status changed - ");Serial.println(status == Sprinkler::Status::On ? "On" : "Off");

			//here we will communicate with the hardware
			// in the lines of - getting the sprinkler id to the sprinkler hardware position (its id can be the same as his pin position)
			// then - pin selector selecet that pin and put there high or low.
			hrdwrctrl::SwitchArray::Switch & switchh = *(_switchHub->getSwitches()[_sprinkler.lock()->id]);
			hrdwrctrl::SwitchArray::Switch::Status switchStatus = status == Sprinkler::Status::On ? hrdwrctrl::SwitchArray::Switch::Status::On : hrdwrctrl::SwitchArray::Switch::Status::Off;
			int retVal = _switchHub->setSwitchStatus(switchh, switchStatus);
			if(retVal != 1){
				Serial.println("__Some error accured in _switchHub->setSwitchStatus sprinkler controller.");
				_sprinkler.lock()->setStatus(status == Sprinkler::Status::On ? Sprinkler::Status::Off : Sprinkler::Status::On);//we failed to change the status so we revert back to the old one, the problem is that in the middle of the on change notification (while in the loop) we will change it again and call the loop again
			}

			Serial.print("Sprinkler - ");Serial.print(_sprinkler.lock()->id.get());
			Serial.print(" Status changed - ");Serial.println(status == Sprinkler::Status::On ? "On" : "Off");
			Serial.println("@@@@@@@@@@ End changing sprinkler status @@@@@@@@@@");

		}

		int getSprinklerPin(){
			return _sprinkler.lock()->id == 1 ? D1 : D8;//TODO: Temporary
		}

	};

	std::shared_ptr<DAL::GardenUnitOfWork> _unitOfWork;
	std::vector<std::shared_ptr<SprinklerController>> _controllers;
	int hookId;
	std::shared_ptr<hrdwrctrl::SwitchArray> _switchHub;
public:
	SprinklersController(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork, std::shared_ptr<hrdwrctrl::SwitchArray> switchHub) : _unitOfWork(unitOfWork), _switchHub(switchHub) {
		for(auto& sprinkler : _unitOfWork->Sprinklers().getAll()) {
			auto sprinklerController = std::make_shared<SprinklerController>(sprinkler, _switchHub);
			_controllers.push_back(sprinklerController);// check if this line do what you think it do
		}
		auto lambdaOnSprinklersChange = [=](std::shared_ptr<GardenModel::Sprinkler> sprinkler, Model::Change change){return this->onSprinklersChange(sprinkler, change);};
		hookId = _unitOfWork->Garden().getById(-1)->_sprinklers.on_change().connect(lambdaOnSprinklersChange);
	}

	virtual ~SprinklersController() {
		_unitOfWork->Garden().getById(-1)->_sprinklers.on_change().disconnect(hookId);
		Serial.print("Number of controllers: ");Serial.println((int)(_controllers.size()));
		Serial.print("Number of sprinklers: ");Serial.println((int)(_unitOfWork->Garden().getById(-1)->_sprinklers.size()));
		_controllers.clear();
	}

	void onSprinklersChange(std::shared_ptr<GardenModel::Sprinkler> sprinkler, Model::Change change) {
		Serial.println(" called onSprinklersChange");
		if(change == Model::Change::Added)
			onNewSprinkler(sprinkler);
		else if(change == Model::Change::Deleted)
			onSprinklerDelete(sprinkler);
	}

	void onNewSprinkler(std::shared_ptr<GardenModel::Sprinkler> sprinkler){
		Serial.println(" onSprinklersChange - Added");
		_controllers.push_back(std::make_shared<SprinklerController>(sprinkler, _switchHub));
	}

	//TODO: make sure that maybe someone above it knows to get this sprinkler out from all the plants which uses it
	// maybe use EMF like validations
	void onSprinklerDelete(std::shared_ptr<GardenModel::Sprinkler> sprinkler){
		Serial.print("onSprinklersChange - Removed, ");
		auto comperator = [&](std::shared_ptr<SprinklerController>& sprinklerController){ return sprinklerController->_sprinkler.lock()->id == sprinkler.get()->id;};
		auto position = std::find_if(_controllers.begin(), _controllers.end(), comperator);
		if (position != _controllers.end()){ // == myVector.end() means the element was not found
			_controllers.erase(position);
			Serial.println("Actually deleted a sprinkler");
		}else
			Serial.println("ERROR? didn't deleted a sprinkler sprinkler 404 in the list");
	}
};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_CONTROLLERS_SPRINKLERSCONTROLLER_H_ */
