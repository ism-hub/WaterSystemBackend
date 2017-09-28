/*
 * Garden.h
 *
 *  Created on: 21 αιεμι 2017
 *      Author: IsM
 */

#ifndef MODEL_GARDEN_H_
#define MODEL_GARDEN_H_

#include <DALModule/serialization/cereal2.h>
#include <vector>
#include <Plant.h>
#include <GardenAcceptable.h>


//#include <se>

using namespace std;

namespace GardenModel {

class Garden : public GardenAcceptable{
public:
	vector<shared_ptr<Plant> > _plants;

	Garden() {//garden with one plant and one sprinkler for that plant
		_plants.reserve(10);
		_plants.push_back(make_shared<Plant>(make_shared<Sprinkler>()));
	}
	virtual ~Garden(){
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ Garden DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	std::weak_ptr<Plant> getPlant(int id) {
		if(_plants.size() - 1 < id)
			return {};//returns empty weak_ptr
		return _plants[id];
	}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor) {
		return visitor.visit(*this);
	}


	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		archive(CEREAL2_NVP(_plants));
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(_plants);
	}

};

} /* namespace GardenModel */

#endif /* MODEL_GARDEN_H_ */
