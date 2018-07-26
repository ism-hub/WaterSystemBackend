/*
 * chipSelectSPIService.h
 *
 *  Created on: 25 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_CHIPS_CHIPSELECTSPISERVICE_H_
#define HARDWAREMODULE_CHIPS_CHIPSELECTSPISERVICE_H_

#include <SPIService.h>
#include <ChipSelect.h>
#include <memory>

namespace hrdwrctrl {

/*class chipSelectSPIService : public ISPIService {
	std::shared_ptr<ChipSelect> chipSelect;
public:

	chipSelectSPIService(std::shared_ptr<ChipSelect> chipSelect = nullptr) : chipSelect(chipSelect) {}
	virtual ~chipSelectSPIService() {}

	void setChipSelect(std::shared_ptr<ChipSelect> chipSelect) {
		this->chipSelect = chipSelect;
	}

	int registerChipOnBoard(const chip& chip) {
		return chipSelect->registerChipOnChipSelect(chip);
	}

	int selectChip(const char* uniqueName) {
		Serial.print("~~~~~~~~~~~ Selecting chip ");Serial.print(uniqueName);Serial.println(" on chip select board ~~~~~~~~~~~");
		int retVal =  chipSelect->selectChip(uniqueName);
		Serial.print("~~~~~~~~~~~ End selecting chip ");Serial.print(uniqueName);Serial.println(" on chip select board ~~~~~~~~~~~");
		return retVal;
	}

	int unselectCurrentChip() {
		Serial.println("~~~~~~~~~~~ Unselecting chips on chip select board ~~~~~~~~~~~");
		int retVal= chipSelect->unselectAll();
		Serial.println("~~~~~~~~~~~ End unselecting chips on chip select board ~~~~~~~~~~~");
		return retVal;
	}

	unsigned char transfer(unsigned char data) {
		Serial.println("~~~~~~~~~~~ Transfering data on chip select board ~~~~~~~~~~~");
		int retVal = chipSelect->transfer(data);
		Serial.println("~~~~~~~~~~~ End transfering data on chip on chip select board ~~~~~~~~~~~");
		return retVal;
	}

};*/

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_CHIPS_CHIPSELECTSPISERVICE_H_ */
