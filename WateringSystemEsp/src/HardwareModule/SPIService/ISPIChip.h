/*
 * ISPIChip.h
 *
 *  Created on: 26 αιεμι 2018
 *      Author: IsM
 */

#ifndef HARDWAREMODULE_SPISERVICE_ISPICHIP_H_
#define HARDWAREMODULE_SPISERVICE_ISPICHIP_H_

#include <SPI.h>
#include <ErrorHandling/Error.h>

namespace hrdwrctrl {

class SPIService;

class ISPIChip {
	SPISettings spiSettings;
	unsigned int csPinNumber;
	SPIService& spiService;
	bool isRegisteredYet = false;// we dont register in the ctor cause register can throw error, so we lazy register (only on transfer)
public:
	ISPIChip(SPISettings spiSettings, unsigned int csPinNumber, SPIService& spiService) :
		spiSettings(spiSettings), csPinNumber(csPinNumber), spiService(spiService)
	{}
	unsigned int getCSPinNumber() {return csPinNumber;}
	const SPISettings getSPISettings() {return spiSettings;}
	err::Error<unsigned int> transfer(unsigned char* data, unsigned int dataSize);
	virtual ~ISPIChip();
};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_SPISERVICE_ISPICHIP_H_ */
