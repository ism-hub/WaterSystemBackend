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
#include <memory>

namespace GardenModel {

class Plant : public GardenAcceptable {
public:

	shared_ptr<Sprinkler> _sprinkler;

	Plant(shared_ptr<Sprinkler> sprinkler)/*: _sprinkler(sprinkler)*/{
		_sprinkler = sprinkler;
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
