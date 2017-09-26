/*
 * Plant.h
 *
 *  Created on: 21 αιεμι 2017
 *      Author: IsM
 */

#ifndef MODEL_PLANT_H_
#define MODEL_PLANT_H_

#include <DALModule/serialization/cereal2.h>
#include <Sprinkler.h>
#include <GardenAcceptable.h>
#include <memory>


namespace GardenModel {

class Plant : public GardenAcceptable {
public:

	shared_ptr<Sprinkler> _sprinkler;

	Plant(shared_ptr<Sprinkler> sprinkler = nullptr)/*: _sprinkler(sprinkler)*/{
	Serial.println("!@#!@# Plant CTRO has been called");
		_sprinkler = sprinkler;
	}

	Plant(Plant&& other)/*: _sprinkler(sprinkler)*/{
			_sprinkler = std::move(other._sprinkler);
			other._sprinkler = nullptr;
		}

	virtual ~Plant(){
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ Plant DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
		_sprinkler.~__shared_ptr();
	}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor){
			return visitor.visit(*this);
		}

	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		archive(CEREAL2_NVP(_sprinkler));
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(_sprinkler);
	}

};

} /* namespace GardenModel */

#endif /* MODEL_PLANT_H_ */
