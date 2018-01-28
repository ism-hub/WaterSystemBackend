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
#include <Model/SimpleProgram.h>

#include <memory>

#include <ModuleFramework/utils.h>

#include <Model/ObserverDesignPattern/Property.hpp>

//###############
#include <rapidjson/stringbuffer.h>
#include <DALModule/serialization/json2.h>

namespace GardenModel {

class Plant : public GardenAcceptable {
public:
	Model::Property<int> id;
	Model::Property<std::shared_ptr<Sprinkler> > _sprinkler;
	Model::Property<String> name;
	Model::Property<std::shared_ptr<SimpleProgram> > _program;

	Plant(std::shared_ptr<Sprinkler> sprinkler = nullptr, std::shared_ptr<SimpleProgram> program = nullptr): id(0), _sprinkler(sprinkler), name("Lily"), _program(program){
#ifdef DEBUG_MY_CODE
		Serial.println("Plant CTOR");
#endif
	}

	/*Plant(Plant&& other){
		Serial.println("Plant CPY_CTOR");
			_sprinkler = std::move(other._sprinkler);
			other._sprinkler = nullptr;
		}*/

	virtual ~Plant(){
#ifdef DEBUG_MY_CODE
		Serial.println("Plant DTOR");
#endif
	}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor){
			return visitor.visit(*this);
		}


	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		archive(CEREAL2_NVP(id), CEREAL2_NVP(name) );
		Serial.println("b4 archive(cereal2::make_nvp(sprinkler,_sprinkler)");
		archive(cereal2::make_nvp("sprinkler",_sprinkler));
		Serial.println("after archive(cereal2::make_nvp(sprinkler,_sprinkler)");
		archive(cereal2::make_nvp("program",_program));
		//archive(cereal2::make_nvp(MF::getTemplateName<Sprinkler>(),_sprinkler));
		//archive(cereal2::make_nvp(MF::getTemplateName<SimpleProgram>(),_program));
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(id ,name);
		archive(_sprinkler);
		archive(_program);
	}

};

} /* namespace GardenModel */

#endif /* MODEL_PLANT_H_ */
