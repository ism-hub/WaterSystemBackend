/*
 * SwitchArray.h
 *
 *  Created on: 19 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_CHIPS_SWITCHARRAY_H_
#define HARDWAREMODULE_CHIPS_SWITCHARRAY_H_

#include <vector>
#include <memory>
#include <ISPIService.h>

namespace hrdwrctrl {

class SwitchArray {
public:
	class Switch {
	public:
		enum Status{On, Off};
	protected:
		const int id;
		Status status;
//		std::shared_ptr<hrdwrctrl::SPIService> _spiService;
	public:

		Switch(int id, Status status = Switch::Off) :
			id(id),
			status(status){}

		virtual ~Switch(){}

		int getId() const {return id;}
		Status getStatus() const {return status;}
		const char* getStatusString() const {return status == Switch::On ? "On" : "Off";}

		//ret - error code, 1 - success
		int setStatus(Status status) {
			this->status = status;
			return 1;
		}
	};

	class SwitchArrayConfig {
	public:
		const hrdwrctrl::chip spiConf;
		const int numberOfSwitches;

		SwitchArrayConfig(hrdwrctrl::chip spiConf, int numberOfSwitches) :
			spiConf(spiConf), numberOfSwitches(numberOfSwitches) {}
	};

protected:
	std::vector<std::shared_ptr<Switch>> switches;
	std::shared_ptr<hrdwrctrl::ISPIService> _spiService;
	SwitchArrayConfig config;

public:
	SwitchArray(std::shared_ptr<hrdwrctrl::ISPIService> spiService, SwitchArrayConfig config) :
		_spiService(spiService),
		config(config) {

		_spiService->registerChipOnBoard(config.spiConf);
		addSwitches();
	}

	virtual ~SwitchArray() {}

	const std::vector<std::shared_ptr<Switch>>& getSwitches() const {
		return switches;
	}

	int setSwitchStatus(Switch& stwitch, Switch::Status status) {
		Serial.print("set switch status ");Serial.println(stwitch.getId());
		if(stwitch.getStatus() == status){
			Serial.print("    status is already ");Serial.print(stwitch.getStatusString());
			return 1;
		}
		//need to select this chip in the chip select
		//need to change this chip output while keeping the rest the same
		_spiService->selectChip(config.spiConf.name);

		//building the byte to send
		char dataToSend = 0;
		if(switches.size() > 8){
			Serial.println("not supporting more than 8 switches rn");
			return 503;
		}
		for(int i=switches.size() - 1; i >= 0; i--){
			dataToSend = dataToSend << 1;//shift
			dataToSend += (switches[i]->getStatus() == Switch::On);
		}
		//setting the new status in the SPI format
		Serial.println("Transfering the  data to the spi (changing the status of the switch)");
		dataToSend = dataToSend^(1 << stwitch.getId());
		_spiService->transfer(dataToSend);

		_spiService->unselectCurrentChip();

		stwitch.setStatus(status);
		return 1;
	}

	void printBitsOfNum(char num){
		char mask = 1;
		for(int i = 0; i<sizeof(num)*8; i++){
			Serial.print((mask&num) != 0 ? "1" : "0");
			mask = mask << 1;
		}
		Serial.println("");
	}

	//set the N first switches state to be the N first bits of
	//data is 0 where the bit number > N
	int setStatusToTheFirstNswitches(char data, int N) {
		if(N > (sizeof(data)*8)){
			Serial.println("We don't have N switches to set");
			return 501;
		}
		Serial.println("inside setStatusToTheFirstNswitches ");


		char systemStatus = 0;
		for(auto& switchh : switches) {
			systemStatus = systemStatus << 1;
			systemStatus += (switchh->getStatus() == SwitchArray::Switch::On);
		}

		Serial.print("We want to set the ");Serial.print(N);Serial.print(" first bits of ");printBitsOfNum(data);
		Serial.print("to ");Serial.print("Current system status ");printBitsOfNum(systemStatus);

		unsigned int maskOnWhatWeWantUnchanged =  ((1 << ((sizeof(data)*8) - N)) - 1) << N ;
		unsigned int statusWeWant = (maskOnWhatWeWantUnchanged & systemStatus)|data;

		_spiService->selectChip(config.spiConf.name);
		int retVal = _spiService->transfer(statusWeWant);
		_spiService->unselectCurrentChip();

		//set the switches to the correct status
		unsigned int tmp = statusWeWant;
		for(auto switchh : switches) {
			switchh->setStatus((tmp & 1) ? SwitchArray::Switch::On : SwitchArray::Switch::Off);
			tmp = tmp >> 1;
		}
		return 1;
	}

private:

	void addSwitches() {
		for(int i=0; i<config.numberOfSwitches; i++){
			switches.push_back(std::make_shared<Switch>(i));
		}
	}

};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_CHIPS_SWITCHARRAY_H_ */
