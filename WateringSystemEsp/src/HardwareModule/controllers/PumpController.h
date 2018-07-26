/*
 * PumpController.h
 *
 *  Created on: 6 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_CONTROLLERS_PUMPCONTROLLER_H_
#define HARDWAREMODULE_CONTROLLERS_PUMPCONTROLLER_H_

#include <memory>
#include <GardenUnitOfWork.h>
#include <SwitchArray.h>

namespace hrdwrctrl {

class PumpController {
	std::shared_ptr<DAL::GardenUnitOfWork> _unitOfWork;
	int pumpStatusChangeHookId;
	int pumpSwitchNumber;
	std::shared_ptr<hrdwrctrl::SwitchArray> _switchHub;
public:
	PumpController(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork, std::shared_ptr<hrdwrctrl::SwitchArray> switchHub, int pumpSwitchNumber) : _unitOfWork(unitOfWork), pumpSwitchNumber(pumpSwitchNumber), _switchHub(switchHub) {
		auto lambdaOnStatusChange = [=](Pump::Status status){return this->onStatusChange(status);};
		pumpStatusChangeHookId = _unitOfWork->Garden().getById(-1)->_pump->status.on_change().connect(lambdaOnStatusChange);
	}
	virtual ~PumpController() {
		_unitOfWork->Garden().getById(-1)->_pump->status.on_change().disconnect(pumpStatusChangeHookId);
	}

	int dryRun = false;
	void onStatusChange(Pump::Status status) {

		if(dryRun){
			Serial.println("__doing dryRun to SprinklersConroller onStatusChange.");
			dryRun = false;
			return;
		}
		//digitalWrite(pumpSwitchNumber, status == Pump::Status::On ? HIGH : LOW);
		Serial.print("Pump status changed - ");Serial.println(status == Pump::Status::On ? "On" : "Off");

		//here we will communicate with the hardware
		Serial.println("@@@@@@@@@@ Changing pump status @@@@@@@@@@");
		hrdwrctrl::SwitchArray::Switch & switchh = *(_switchHub->getSwitches()[pumpSwitchNumber]);
		hrdwrctrl::SwitchArray::Switch::Status switchStatus = status == Pump::Status::On ? hrdwrctrl::SwitchArray::Switch::Status::On : hrdwrctrl::SwitchArray::Switch::Status::Off;
		int retVal = _switchHub->setSwitchStatus(switchh, switchStatus);
		if(retVal != 1){
			Serial.println("__Some error accured in _switchHub->setSwitchStatus inside pump controller.");
			_unitOfWork->Garden().getById(-1)->_pump->status = (status == Pump::Status::On ? Pump::Status::Off : Pump::Status::On);//we failed to change the status so we revert back to the old one, the problem is that in the middle of the on change notification (while in the loop) we will change it again and call the loop again
		}else {
			Serial.print("Pump status changed - ");Serial.println(status == Pump::Status::On ? "On" : "Off");
		}
		Serial.println("@@@@@@@@@@ End changing pump status @@@@@@@@@@");

	}


};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_CONTROLLERS_PUMPCONTROLLER_H_ */
