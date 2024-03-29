/*
 * asd.cpp
 *
 *  Created on: 26 ����� 2018
 *      Author: IsM
 */

#include <HardwareLib/SPIService/SPIService.h>

namespace hrdwrctrl {


err::Error<unsigned int> ISPIChip::transfer(unsigned char* data, unsigned int dataSize) {
	if(!isRegisteredYet){
		err::Error<bool> err = spiService.registerSPIChip(this);
		if(err != err::ErrorCode::NO_ERROR)
			return err;
		isRegisteredYet = true;
	}
	err::Error<unsigned int> iErr = spiService.transfer(*this, data, dataSize);
	if(iErr != err::ErrorCode::NO_ERROR)
		return iErr;
	return 0;
}

ISPIChip::~ISPIChip() {
	spiService.unregisterSPIChip(*this);
}


} /* namespace err */
