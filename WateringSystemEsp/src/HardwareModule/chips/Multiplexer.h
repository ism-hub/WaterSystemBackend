/*
 * Multiplexer.h
 *
 *  Created on: 19 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_CHIPS_MULTIPLEXER_H_
#define HARDWAREMODULE_CHIPS_MULTIPLEXER_H_

#include <HardwareModule/chips/SwitchArray.h>


namespace hrdwrctrl {

class Multiplexer {
	SwitchArray multiplexerInterface;// this thing controls the multiplexer, (choose which pin we want)
public:
	class MultiplexerConfig {
	public:
		const int numOfPins;
		const chip spiConf;
	};

	const MultiplexerConfig config;

	Multiplexer(MultiplexerConfig config) : config(config){}
	virtual ~Multiplexer(){}

	int readFromPinNumber(int pinNumber) {
		if(pinNumber > config.numOfPins) {
			Serial.println("Multiplexer want read from pin we don't have.");
			return 404;
		}
		multiplexerInterface.setStatusToTheFirstNswitches(pinNumber, config.numOfPins);
		return 1;
	}

};

} /* namespace hrdwrctrl */

#endif /* HARDWAREMODULE_CHIPS_MULTIPLEXER_H_ */
