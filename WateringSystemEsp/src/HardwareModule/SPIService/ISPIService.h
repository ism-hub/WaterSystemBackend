/*
 * ISPIService.h
 *
 *  Created on: 25 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_SPISERVICE_ISPISERVICE_H_
#define HARDWAREMODULE_SPISERVICE_ISPISERVICE_H_

#include <stdio.h>

namespace hrdwrctrl {

//contains the parameters the spi need to run this chip. and the chip name to identify him.
struct chip {
	friend class SPIService;
	const char* name="";
	int selectPin=-1;
	//here maybe we will have specific configuration of the SPI for that chip
	chip(const char* name, int selectPin) : name(name), selectPin(selectPin){
	}
	chip(){}

	bool operator ==(const chip& other) const {
		return strcmp(name, other.name) == 0;
	}

	bool operator !=(const chip& other) const {
		return !(*this == other);
	}
protected:
	//bool isOnChipSelect=true;
};


class ISPIService {
public:
	ISPIService() {}
	virtual ~ISPIService() {}

	virtual int registerChipOnBoard(const chip& chip) = 0;
	virtual int selectChip(const char* uniqueName) = 0;
	virtual int unselectCurrentChip() = 0;
	virtual unsigned char transfer(unsigned char data) = 0;

};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_SPISERVICE_ISPISERVICE_H_ */
