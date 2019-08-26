/*
 * ISPIService.h
 *
 *  Created on: 25 ���� 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_SPISERVICE_ISPISERVICE_H_
#define HARDWAREMODULE_SPISERVICE_ISPISERVICE_H_

#include <stdio.h>
#include <HardwareLib/SPIService/ISPIChip.h>
#include <ErrorHandling/Error.h>
#include <utility>      // std::pair, std::make_pair
#include <SPI.h>
#include <memory>

namespace hrdwrctrl {


class SPIService {
	//registeredChips[i] means the ISPIChip that is connected to the i'th chip select pin
	std::vector<ISPIChip*> registeredChips;
	const unsigned int numberOfChipSelectPins;
	const SPISettings spiBoardSettings = SPISettings(500, MSBFIRST, SPI_MODE0);//the settings the on board SPI talks to our SPI manager chip
	//unsigned int delayTime = 100;
public:
	//@pre: numberOfChipSelectPins%8 == 0
	SPIService(unsigned int numberOfChipSelectPins) :
		registeredChips(numberOfChipSelectPins), numberOfChipSelectPins(numberOfChipSelectPins)
	{
		//initiate the chip select of the board to not selected
		pinMode(SS, OUTPUT);
		digitalWrite(SS, HIGH);
	}

	SPIService & operator=(const SPIService&) = delete;
	SPIService(const SPIService&) = delete;
	SPIService() = delete;

	~SPIService() {
	//	Serial.print("AHHHHHHHHHHHHHHHHHHHHHHHHAHHHHHHHHHHHHHHHHHHHHHHHH AHHHHHHHHHHHHH");
	}

	err::Error<bool> registerSPIChip(ISPIChip* spiChip) {
		unsigned int CSPinNumber = spiChip->getCSPinNumber();
		if(!isChipSelectInRange(CSPinNumber))
			return err::Error<int>(err::ErrorCode::ERR_SPI_CS_INVALID, String(CSPinNumber));
		if(isChipSelectPinAlreadyTaken(CSPinNumber))
			return err::Error<int>(err::ErrorCode::ERR_SPI_CS_TAKEN, String(CSPinNumber));
		registeredChips[CSPinNumber] = spiChip;
		return true;
	}

	err::Error<bool> unregisterSPIChip(ISPIChip& spiChip) {
		unsigned int CSPinNumber = spiChip.getCSPinNumber();
		if(!isChipSelectInRange(CSPinNumber))
			return err::Error<int>(err::ErrorCode::ERR_SPI_CS_INVALID, String(CSPinNumber));
		if(!isChipSelectPinAlreadyTaken(CSPinNumber))
			return err::Error<int>(err::ErrorCode::ERR_SPI_CS_ALREADYFREE, String(CSPinNumber));
		registeredChips[CSPinNumber] = NULL;
		return true;

	}

	//if isChipSelectInRange(chipSelectPin) == false we return true (like that chip is already taken)
	bool isChipSelectPinAlreadyTaken(unsigned int chipSelectPin){
		if(!isChipSelectInRange(chipSelectPin))
			return true;
		if(registeredChips[chipSelectPin] != NULL)
			return true;
		return false;
	}

	bool isChipSelectInRange(unsigned int chipSelectPin){
		return chipSelectPin < this->numberOfChipSelectPins;
	}

	//transfer byte by byte to ISPIChip
	err::Error<unsigned int> transfer(ISPIChip& spiChip, unsigned char* data, unsigned int dataSize){
		SPI.begin();
		err::Error<bool> err = selectChip(spiChip.getCSPinNumber());
		if(err != err::ErrorCode::NO_ERROR)
			return err;

		//transfer the data
		//Serial.print("The dataToSend size is:");Serial.print(dataSize);Serial.println("Bytes");
		//Serial.print("The first byte we transfer to the chip : ");
		//printBitsOfNum(data[0]);
		SPI.beginTransaction(spiChip.getSPISettings());
		for(unsigned int i = 0; i < dataSize; i++)
			SPI.transfer(data[i]);
		SPI.endTransaction();

		//delay(delayTime);

		err = unselectAllChips();
		if(err != err::ErrorCode::NO_ERROR)
			return err;
		SPI.end();
		return 0;
	}

	err::Error<bool> unselectAllChips(){
		//prepare the data to send
		unsigned int numberOfBytes = numberOfChipSelectPins/8;
		char dataToSend[numberOfBytes];
		for(unsigned int i = 0; i < numberOfBytes; i++)
			dataToSend[i] = 0xFF;

		err::Error<unsigned int> uiErr = transferToTheSPIBoard(dataToSend, numberOfBytes);
		if(uiErr.errorCode != err::ErrorCode::NO_ERROR)
			return uiErr;
		return true;
	}

protected:
	err::Error<bool> selectTheSPIBoard(){
		digitalWrite(SS, LOW);//low is selected
		//delay(delayTime);
		return true;
	}
	err::Error<bool> unselectTheSPIBoard(){
		digitalWrite(SS, HIGH);//unselect the board
		//delay(delayTime);
		return true;
	}

	//select it -> transer -> unselect the board
	err::Error<unsigned int> transferToTheSPIBoard(char* data, unsigned int dataSize = 1){
		SPI.begin();
		SPI.beginTransaction(spiBoardSettings);
		selectTheSPIBoard();

		//Serial.print("The dataToSend size is:");Serial.print(dataSize);Serial.println("Bytes");
		//Serial.print("The first byte we transfer to the spi board : ");
		//printBitsOfNum(data[0]);

		for(unsigned int i = 0; i < dataSize; i++)
			SPI.transfer(data[i]);
		//delay(delayTime);
		SPI.endTransaction();
		unselectTheSPIBoard();
		//if(bitsSent != dataSize*8)
		//	return err::ErrorCode::ERR_SPI_FAILED_SENDDATA;
		return 0;
	}

	void printBitsOfNum(unsigned char num){
		char mask = 1;
		for(unsigned int i = 0; i<sizeof(num)*8; i++){
			Serial.print((mask&num) != 0 ? "1" : "0");
			mask = mask << 1;
		}
		Serial.println("");
	}

	//select a chip on the board (low = selected)
	err::Error<bool> selectChip(unsigned int chipSelectPin){

		if(!isChipSelectInRange(chipSelectPin)){
			err::Error<bool> err(err::ErrorCode::ERR_SPI_CS_INVALID, String("" + chipSelectPin));
			return err;
		}
		if(!isChipSelectPinAlreadyTaken(chipSelectPin)){
			err::Error<bool> err(err::ErrorCode::ERR_SPI_CS_ALREADYFREE, String("" + chipSelectPin));
			return err;
		}
		//prepare the data to send
		unsigned int numberOfBytes = numberOfChipSelectPins/8;
		printBitsOfNum((0x01 << chipSelectPin%8) ^ 0xFF);
		char dataToSend[numberOfBytes];
		for(unsigned int i = 0; i < numberOfBytes; i++)
			if(i == chipSelectPin/8)
				dataToSend[i] = (0x01 << chipSelectPin%8) ^ 0xFF;
			else
				dataToSend[i] = 0xFF;

		err::Error<unsigned int> uiErr = transferToTheSPIBoard(dataToSend, numberOfBytes);
		if(uiErr.errorCode != err::ErrorCode::NO_ERROR)
			return uiErr;
		return true;
	}
};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_SPISERVICE_ISPISERVICE_H_ */