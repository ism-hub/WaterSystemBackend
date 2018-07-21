/*
 * SPIService.h
 *
 *  Created on: 19 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_SPISERVICE_SPISERVICE_H_
#define HARDWAREMODULE_SPISERVICE_SPISERVICE_H_

#include <SPI.h>
#include <algorithm>
#include <ISPIService.h>

namespace hrdwrctrl {


//the SPIService have set of chips and every chip has his pin select and his configuration
//we can register chip, select it (will start the SPI with the correct conf for the chip), transfer/read data
//class to centralize all the chips which connect to the SPI (all the connections are from here and we can make sure
//that every chip has its own chipSelectPin and that the SPI is only accessible from here)
class SPIService : public ISPIService {
	chip emptyChip{"", -1};
	//std::vector<chip> chipsOnChipSelect;
	std::vector<chip> chipsOnBoard;
	chip* currSelectedChip = &emptyChip;

	//std::shared_ptr<ChipSelect> chipSelect;

public:
	SPIService(){}
	virtual ~SPIService() {}


	//ret - 1: success, o.w: error code
	//selectPin - selects pin to on, on the board itself
	int registerChipOnBoard(const chip& chip) {
		hrdwrctrl::chip& chip2 = getChipWithName(chip.name);
		if(chip2 != emptyChip){
			Serial.print("Failed to register chip, chip is already exist - ");Serial.println(chip.name);
			return 0;
		}
		Serial.print("Registering new chip - ");Serial.println(chip.name);
		chipsOnBoard.push_back(chip);
		return 1;
	}


	//Turns on the specific pin,
	//the pin can be from our chipSelect pin or directly on our SPI
	//ret - 1: success, o.w: error code
	int selectChip(const char* uniqueName) {
		Serial.print("Selecting chip ");Serial.print(uniqueName);Serial.print(" on pin ");
		chip& chip = getChipWithName(uniqueName);
		if(chip == emptyChip){
			Serial.print("Failed to select chip, chip not exist - ");Serial.println(uniqueName);
			return 404;
		}
		Serial.println(chip.selectPin);
		return selectChipInner(chip);
	}

protected:
	int selectChipInner(chip& chip) {
		//here we will turn the SPI on with the correct configuration
		int retVal;
		Serial.print("Setting pin number ");Serial.print(chip.selectPin);Serial.println(" on board to select state");
		//TODO: set the pin number here

		currSelectedChip = &chip;
		return 1;
	}
public:

	//ret - 1: success, o.w: error code
	//int unselectChip(const char* name) {
	//	currSelectedChip = &emptyChip;
	//	return 1;
	//}

	int unselectCurrentChip() {
		Serial.print("Setting pin number ");Serial.print(currSelectedChip->selectPin);Serial.println(" on board to unselected state");
		currSelectedChip = &emptyChip;
		return 1;
	}

	unsigned char transfer(unsigned char data) {
		//SPISettings SPISettings{clock, bitOrder, dataMode};
		SPI.begin();
		unsigned char recvData = SPI.transfer(data);
		SPI.end();
		return recvData;
	}

private:
	chip& getChipWithName(const char* uniqueName ) {
		auto comperator = [=](const chip& other){return strcmp(uniqueName, other.name) == 0; };
		auto it = std::find_if(chipsOnBoard.begin(), chipsOnBoard.end(), comperator);
		if(it != chipsOnBoard.end())
			return *it;

		return emptyChip;
	}
};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_SPISERVICE_SPISERVICE_H_ */
