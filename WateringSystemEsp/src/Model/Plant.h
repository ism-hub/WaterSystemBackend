/*
 * Plant.h
 *
 *  Created on: 21 αιεμι 2017
 *      Author: IsM
 */

#ifndef MODEL_PLANT_H_
#define MODEL_PLANT_H_

#include <Sprinkler.h>
#include <GardenAcceptable.h>

namespace GardenModel {

class Plant : public GardenAcceptable {
public:

	Sprinkler* _sprinkler;

	Plant(Sprinkler* sprinkler): _sprinkler(sprinkler){

	}

	virtual ~Plant(){
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ Plant DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	virtual void* accept(GardenVisitor& visitor){
			return visitor.visit(*this);
		}

};

} /* namespace GardenModel */

#endif /* MODEL_PLANT_H_ */
