/*
 * Pump.h
 *
 *  Created on: 7 במרץ 2018
 *      Author: rina
 */

#ifndef MODEL_PUMP_H_
#define MODEL_PUMP_H_

#include <ObserverDesignPattern/Property.hpp>

namespace garden {

class Pump {
public:
	enum Status { Off, On };
		//String name;
	Model::Property<Status> status = Off;

	Pump() {}
	 ~Pump() {}
};

} /* namespace GardenModel */

#endif /* MODEL_PUMP_H_ */
