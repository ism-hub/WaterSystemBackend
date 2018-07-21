/*
 * SPI.h
 *
 *  Created on: 20 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWARETESTS_SPI_H_
#define HARDWARETESTS_SPI_H_


class SPIMock {
public:
	void begin(){
		std::cout << "SPI.begin() has been called" << std::endl;
	}

	void end(){
		std::cout << "SPI.end() has been called" << std::endl;
	}

	char transfer(char data){
		std::cout << "SPI.transfer(data) has been called " << std::endl;
		int mask = 1;
		for(int i=0; i < sizeof(data)*8; i++){
			Serial.print((mask&data) != 0 ? "1" : "0");
			mask = mask << 1;
		}
		std::cout << std::endl;
		std::cout << ((int)data) << std::endl;
		return 1;
	}
};

SPIMock SPI;

#endif /* HARDWARETESTS_SPI_H_ */
