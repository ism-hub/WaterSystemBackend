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

#include <ModuleFramework/utils.h>


//#include <se>

using namespace std;

namespace GardenModel {

class Garden : public GardenAcceptable{
public:
	vector<shared_ptr<Plant> > _plants;

	Garden() {
		Serial.println("Garden CTOR");
	}
	virtual ~Garden(){
		Serial.println("Garden DTOR");
	}

	std::shared_ptr<Plant> getPlant(int id) {
		if(_plants.size() - 1 < id)
			return {};
		return _plants[id];
	}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor) {
		return visitor.visit(*this);
	}


	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		archive(cereal2::make_nvp(MF::getTemplateName<Plant>(),_plants));
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(_plants);
	}

};

} /* namespace GardenModel */

#endif /* MODEL_GARDEN_H_ */
