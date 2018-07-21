/*
 * ChipSelect.h
 *
 *  Created on: 19 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_CHIPS_CHIPSELECT_H_
#define HARDWAREMODULE_CHIPS_CHIPSELECT_H_

#include <SPIService.h>
#include <SwitchArray.h>



namespace hrdwrctrl {

//ChipSelect chip implemented with switch array that we make sure only one switch is up in a given time
class ChipSelect {

	std::vector<chip> chips;
	chip emptyChip{"", -1};
	std::shared_ptr<SPIService> _spiService;
	//chip* currSelectedChip = &emptyChip;
public:

	class ChipSelectConfig {
	public:
		const int numOfPins;
		chip spiConf;

		ChipSelectConfig(int numOfPins, chip spiConf) :
			numOfPins(numOfPins), spiConf(spiConf) {}
	};

protected:
	const ChipSelectConfig config;
	SwitchArray switchArray;
public:
	ChipSelect(std::shared_ptr<SPIService> spiService, ChipSelectConfig config) :
		_spiService(spiService),
		config(config),
		switchArray(spiService, SwitchArray::SwitchArrayConfig(config.spiConf, config.numOfPins)) {

		//_spiService->registerChipOnBoard(config.spiConf); already in switchArray(...)
	}
	virtual ~ChipSelect(){}

	/*char transferDataToChip(char data, const char* name) {
		char retVal = _spiService->transfer(data);//our chip select have some chip selected, our spi have nothing selected, so when we transfer the data only the one who selected by the chip select will read it.
		unselectAll();// we done the transfer so we tell the spi to select the chipSelect (when the chip select is selected it blocks the clock of the SPI to the rest of the chips on the board, so we can change the chipSelect status without sending data to the chips that connected to it)
		return retVal;
	}*/

	unsigned char transfer(unsigned char data) {
		return _spiService->transfer(data);
	}

	//ret - 1: success, o.w: error code
		//selectPin - the select pin we need to on to select our chip
	int registerChipOnChipSelect(const chip& chip) {
		hrdwrctrl::chip& chip2 = getChipWithName(chip.name);
		if(chip2 != emptyChip){
			Serial.print("Failed to register chip (on chipSelect board), chip is already exist - ");Serial.println(chip.name);
			return 0;
		}
		Serial.print("Registering new chip (on chipSelect board) - ");Serial.println(chip.name);
		chips.push_back(chip);
		return 1;
	}

	int selectChip(const char* uniqueName) {
		chip& chip = getChipWithName(uniqueName);
		if(chip == emptyChip){
			Serial.print("__Didn't find chip named ");Serial.print(uniqueName);Serial.print(" in ChipSelect transfer function");
			return 204;
		}
		return selectPinNumber(chip.selectPin);//after we done with the select the spi will unselect our board
	}

	//set all the pins to low
	int unselectAll() {
		return switchArray.setStatusToTheFirstNswitches(0, config.numOfPins);
	}

protected:
	//set the specific pin number to high, set the rest to low
	int selectPinNumber(unsigned int pinNumber) {
		if(pinNumber >= config.numOfPins) {
			Serial.print("__We only have ");Serial.print(config.numOfPins);Serial.print(" and we are trying to set pin number ");Serial.println(pinNumber);
			return 404;
		}
		return switchArray.setStatusToTheFirstNswitches(1 << (pinNumber - 1), config.numOfPins);
	}



private:
	chip& getChipWithName(const char* uniqueName ) {
		auto comperator = [=](const chip& other){return strcmp(uniqueName, other.name) == 0; };
		auto it = std::find_if(chips.begin(), chips.end(), comperator);
		if(it != chips.end())
			return *it;

		return emptyChip;
	}
};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_CHIPS_CHIPSELECT_H_ */
