/*
 * Garden.h
 *
 *  Created on: 21 αιεμι 2017
 *      Author: IsM
 */

#ifndef MODEL_GARDEN_H_
#define MODEL_GARDEN_H_

#include <vector>
#include <Plant.h>
#include <GardenAcceptable.h>

using namespace std;

namespace GardenModel {

class Garden : public GardenAcceptable{
public:
	vector<Plant*> _plants;

	Garden() {//garden with one plant and one sprinkler for that plant
		_plants.reserve(10);
		Sprinkler* sprinkler = new Sprinkler();
		_plants.push_back(new Plant(sprinkler));
	}
	virtual ~Garden(){
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ Garden DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	Plant* getPlant(int id) {
		if(_plants.size() - 1 < id)
			return NULL;
		return _plants[id];
	}

	virtual void* accept(GardenVisitor& visitor){
			return visitor.visit(*this);
	}


};

} /* namespace GardenModel */

#endif /* MODEL_GARDEN_H_ */
