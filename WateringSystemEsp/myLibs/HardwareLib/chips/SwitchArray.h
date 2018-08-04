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
#include <HardwareLib/SPIService/SPIService.h>

namespace hrdwrctrl {

class SwitchArray : protected ISPIChip{
public:
	class Switch {
	public:
		enum class Status{On, Off};
	protected:
		const int id;
		Status status;
	public:
		Switch(int id, Status status = Status::Off) :
			id(id),
			status(status){}

		virtual ~Switch(){}

		int getId() const {return id;}
		Status getStatus() const {return status;}
		const char* getStatusString() const {return status == Status::On ? "On" : "Off";}

		void setStatus(Status status) {
			this->status = status;
		}
	};

	class SwitchArrayConfig {
	public:
		//spiManagerConfig
		SPISettings spiSettings;
		unsigned int csPinNumber;

		const unsigned int numberOfSwitches;

		SwitchArrayConfig(SPISettings spiSettings, unsigned int csPinNumber, unsigned int numberOfSwitches) :
			spiSettings(spiSettings), csPinNumber(csPinNumber), numberOfSwitches(numberOfSwitches) {}
	};

protected:
	std::vector<std::shared_ptr<Switch>> switches;
	//std::shared_ptr<hrdwrctrl::SPIService> _spiService;
	SwitchArrayConfig config;

public:
	SwitchArray(std::shared_ptr<hrdwrctrl::SPIService> spiService, SwitchArrayConfig config) :
		ISPIChip(config.spiSettings, config.csPinNumber, *spiService),
		config(config) {
		addSwitches();
	}

	void resetState(){
		for(auto& switchh : switches)
			switchh->setStatus(Switch::Status::Off);
		unsigned char data = 0x00;
		transfer(&data, 1);
	}

	virtual ~SwitchArray() {}

	const std::vector<std::shared_ptr<Switch>>& getSwitches() const {
		return switches;
	}

	int setSwitchStatus(Switch& stwitch, Switch::Status status) {
	//	Serial.print("set switch status ");Serial.println(stwitch.getId());
		if(stwitch.getStatus() == status){
	//		Serial.print("    status is already ");Serial.print(stwitch.getStatusString());
			return 1;
		}

		//building the byte to send
		unsigned char dataToSend = 0;
		if(switches.size() > 8){
			Serial.println(F("not supporting more than 8 switches rn"));
			return 503;
		}
		for(int i=switches.size() - 1; i >= 0; i--){
			dataToSend = dataToSend << 1;//shift
			dataToSend += (switches[i]->getStatus() == Switch::Status::On);
		}
		//setting the new status in the SPI format
	//	Serial.println("Transfering the  data to the spi (changing the status of the switch)");
		dataToSend = dataToSend^(1 << stwitch.getId());
	//	Serial.print("the data we are sending:");
	//	printBitsOfNum(dataToSend);
	//	Serial.println("b4 transfer(&dataToSend, 1)");
		transfer(&dataToSend, 1);//TODO:: handle error
	//	Serial.println("after transfer(&dataToSend, 1)");
		stwitch.setStatus(status);
		return 1;
	}

	void printBitsOfNum(char num){
		char mask = 1;
		for(unsigned int i = 0; i<sizeof(num)*8; i++){
			Serial.print((mask&num) != 0 ? "1" : "0");
			mask = mask << 1;
		}
		Serial.println("");
	}

	//set the N first switches state to be the N first bits of
	//data is 0 where the bit number > N
	/*int setStatusToTheFirstNswitches(char data, int N) {
		if(N > (sizeof(data)*8)){
			Serial.println("We don't have N switches to set");
			return 501;
		}
		Serial.println("inside setStatusToTheFirstNswitches ");


		char systemStatus = 0;
		for(auto& switchh : switches) {
			systemStatus = systemStatus << 1;
			systemStatus += (switchh->getStatus() == SwitchArray::Switch::Status::On);
		}

		Serial.print("We want to set the ");Serial.print(N);Serial.print(" first bits of ");printBitsOfNum(data);
		Serial.print("to ");Serial.print("Current system status ");printBitsOfNum(systemStatus);

		unsigned int maskOnWhatWeWantUnchanged =  ((1 << ((sizeof(data)*8) - N)) - 1) << N ;
		unsigned int statusWeWant = (maskOnWhatWeWantUnchanged & systemStatus)|data;


		_spiService->transfer(statusWeWant);


		//set the switches to the correct status
		unsigned int tmp = statusWeWant;
		for(auto switchh : switches) {
			switchh->setStatus((tmp & 1) ? SwitchArray::Switch::Status::On : SwitchArray::Switch::Status::Off);
			tmp = tmp >> 1;
		}
		return 1;
	}*/

private:

	void addSwitches() {
		for(unsigned int i=0; i < config.numberOfSwitches; i++){
			switches.push_back(std::make_shared<Switch>(i));
		}
	}

};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_CHIPS_SWITCHARRAY_H_ */
