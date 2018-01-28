/*
 * Garden.h
 *
 *  Created on: 21 ����� 2017
 *      Author: IsM
 */

#ifndef MODEL_GARDEN_H_
#define MODEL_GARDEN_H_

#include <DALModule/serialization/cereal2.h>
#include <vector>
#include <Plant.h>
#include <GardenAcceptable.h>
#include <Model/SimpleProgram.h>

#include <ModuleFramework/utils.h>

#include <Model/ObserverDesignPattern/ObservableVector.h>


//#include <se>



namespace GardenModel {

class Garden : public GardenAcceptable{
public:
	Model::Property<String> name = String("GardName");
	Model::ObservableVector<Model::Property<std::shared_ptr<Plant>> > _plants;
	Model::ObservableVector<Model::Property<std::shared_ptr<Sprinkler>> > _sprinklers;
	Model::ObservableVector<Model::Property<std::shared_ptr<SimpleProgram>> > _programs;

	Garden() {
#ifdef DEBUG_MY_CODE
		Serial.println("Garden CTOR");
#endif
	}
	virtual ~Garden(){
#ifdef DEBUG_MY_CODE
		Serial.println("Garden DTOR");
#endif
	}

	std::shared_ptr<Plant> getPlant(int id) {
		if(_plants.getInnerVector().size() - 1 < id)
			return {};
		return _plants.getInnerVector()[id];
	}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor) {
		return visitor.visit(*this);
	}


	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		archive(CEREAL2_NVP(name));
		archive(cereal2::make_nvp("plants",_plants.getInnerVector()));
		archive(cereal2::make_nvp("sprinklers", _sprinklers.getInnerVector()));
		archive(cereal2::make_nvp("programs",_programs.getInnerVector()));
		//archive(cereal2::make_nvp(MF::getTemplateName<Plant>(),_plants.getInnerVector()));
		//archive(cereal2::make_nvp(MF::getTemplateName<Sprinkler>(), _sprinklers.getInnerVector()));
		//archive(cereal2::make_nvp(MF::getTemplateName<SimpleProgram>(),_programs.getInnerVector()));
	}

	template<class Archive>
	void load(Archive& archive) {
		std::vector<Model::Property<std::shared_ptr<Plant>> > plants;
		std::vector<Model::Property<std::shared_ptr<Sprinkler>> > sprinklers;
		std::vector<Model::Property<std::shared_ptr<SimpleProgram>> > programs;
		archive(name, plants, sprinklers, programs);
		_plants = plants;//TODO: not efficient
		_sprinklers = sprinklers;
		_programs = programs;

		correctPointers();
	}

private:
	void correctPointers(){
		for(const Model::Property<std::shared_ptr<Plant>> & plant : _plants.getInnerVector()){

			for(const Model::Property<std::shared_ptr<Sprinkler>>& sprinkler : _sprinklers.getInnerVector())
				if( plant.get()->_sprinkler.get()->id == sprinkler.get()->id ){
					plant.get()->_sprinkler = sprinkler;
					break;
				}

			for(const Model::Property<std::shared_ptr<SimpleProgram>>& program : _programs.getInnerVector())
				if( plant.get()->_program.get()->id == program.get()->id ){
					plant.get()->_program = program;
					break;
				}
		}
	}

};

} /* namespace GardenModel */

#endif /* MODEL_GARDEN_H_ */
