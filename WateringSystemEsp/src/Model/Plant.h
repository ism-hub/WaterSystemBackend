/*
 * Plant.h
 *
 *  Created on: 21 αιεμι 2017
 *      Author: IsM
 */

#ifndef MODEL_PLANT_H_
#define MODEL_PLANT_H_

//#include <DALModule/serialization/cereal2.h>
#include <Sprinkler.h>
#include <GardenAcceptable.h>
#include <SimpleProgram.h>

#include <memory>

//#include <ModuleFramework/utils.h>

#include <ObserverDesignPattern/Property.hpp>

//###############
//#include <rapidjson/stringbuffer.h>
//#include <DALModule/serialization/json2.h>

namespace GardenModel {

class Plant : public GardenAcceptable {
public:
	Model::Property<int> id;
	Model::Property<std::shared_ptr<Sprinkler> > _sprinkler;
	Model::Property<String> name;
	Model::Property<std::shared_ptr<SimpleProgram> > _program;
	Garden* garden = NULL;//when we add the plant to the garden the garden will set that TODO:When revisiting the model correctness - try to look on the EMF project and see how they handled that double relationship (child have apointer to the parent too)
	//XXX: 111111111 ASAP see above, omfg "Garden* garden;"

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

	void setProgram(int pid);//is it makes sense to the plant to have its garden and to the garden to set the plant program?

	void setSprinkler(int sid);

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor){
			return visitor.visit(*this);
		}


	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		archive.addProperties(MACRO_NVP(id), MACRO_NVP(name) );
		//Serial.println("b4 archive(cereal2::make_nvp(sprinkler,_sprinkler)");
		archive.addProperties(mycereal::make_nvp("sprinkler",_sprinkler));
		//Serial.println("after archive(cereal2::make_nvp(sprinkler,_sprinkler)");
		archive.addProperties(mycereal::make_nvp("program",_program));
		//archive(cereal2::make_nvp(MF::getTemplateName<Sprinkler>(),_sprinkler));
		//archive(cereal2::make_nvp(MF::getTemplateName<SimpleProgram>(),_program));
	}

	template<class Archive>
	void load(Archive& archive) {
		archive.loadProperties(MACRO_NVP(id), MACRO_NVP(name));
		archive.loadProperties(mycereal::make_nvp("sprinkler",_sprinkler));
		archive.loadProperties(mycereal::make_nvp("program",_program));
	}

};

} /* namespace GardenModel */

#endif /* MODEL_PLANT_H_ */
