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

#include <ModuleFramework/utils.h>

namespace GardenModel {

class Plant : public GardenAcceptable {
public:
	int id = 0;
	shared_ptr<Sprinkler> _sprinkler;
	String name = "Lily";

	Plant(shared_ptr<Sprinkler> sprinkler = nullptr): _sprinkler(sprinkler){
	Serial.println("Plant CTOR");
	}

	/*Plant(Plant&& other){
		Serial.println("Plant CPY_CTOR");
			_sprinkler = std::move(other._sprinkler);
			other._sprinkler = nullptr;
		}*/

	virtual ~Plant(){
		Serial.println("Plant DTOR");
		_sprinkler.~__shared_ptr();
	}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor){
			return visitor.visit(*this);
		}

	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		archive(CEREAL2_NVP(id), CEREAL2_NVP(name), cereal2::make_nvp(MF::getTemplateName<Sprinkler>(),_sprinkler));
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(id ,name, _sprinkler);
	}

};

} /* namespace GardenModel */

#endif /* MODEL_PLANT_H_ */
