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
#include <SimpleProgram.h>
#include <algorithm>

//#include <ModuleFramework/utils.h>

#include <ObserverDesignPattern/ObservableVector.h>


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
		auto& v = _plants.getInnerVector();
		auto fnComperator = [id] (const Model::Property<std::shared_ptr<Plant>>& plant) { return plant.get()->id == id; };
		auto it = std::find_if(std::begin(v), std::end(v), fnComperator);

		if(it != std::end(v))
			return *it;
		return nullptr;
	}

	std::shared_ptr<SimpleProgram> getProgram(int id) {//TODO: awfully remiends the repository responsibility, maybe we need to use it here?
		auto& v = _programs.getInnerVector();
		auto fnComperator = [id] (const Model::Property<std::shared_ptr<SimpleProgram>>& plant) { return plant.get()->id == id; };
		auto it = std::find_if(std::begin(v), std::end(v), fnComperator);

		if(it != std::end(v))
			return *it;
		return nullptr;
	}

	std::shared_ptr<Sprinkler> getSprinkler(int id) {
		auto& v = _sprinklers.getInnerVector();
		auto fnComperator = [id] (const Model::Property<std::shared_ptr<Sprinkler>>& sprinkler) { return sprinkler.get()->id == id; };
		auto it = std::find_if(std::begin(v), std::end(v), fnComperator);

		if(it != std::end(v))
			return *it;
		return nullptr;
	}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor) {
		return visitor.visit(*this);
	}


	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		archive.addProperties(mycereal::make_nvp("name", name));
		archive.addProperties(mycereal::make_nvp("plants",_plants.getInnerVector()));
		archive.addProperties(mycereal::make_nvp("sprinklers", _sprinklers.getInnerVector()));
		archive.addProperties(mycereal::make_nvp("programs",_programs.getInnerVector()));
		//archive(cereal2::make_nvp(MF::getTemplateName<Plant>(),_plants.getInnerVector()));
		//archive(cereal2::make_nvp(MF::getTemplateName<Sprinkler>(), _sprinklers.getInnerVector()));
		//archive(cereal2::make_nvp(MF::getTemplateName<SimpleProgram>(),_programs.getInnerVector()));
	}

	template<class Archive>
	void load(Archive& archive) {
		/*std::vector<Model::Property<std::shared_ptr<Plant>> > plants;
		std::vector<Model::Property<std::shared_ptr<Sprinkler>> > sprinklers;
		std::vector<Model::Property<std::shared_ptr<SimpleProgram>> > programs;
		archive.loadProperties(MACRO_NVP(name), MACRO_NVP(sprinklers), MACRO_NVP(programs));
		_sprinklers = sprinklers;//TODO: not efficient
		_programs = programs;
		archive.loadProperties(MACRO_NVP(plants));//the order is important, the plants need the sprinklers and programs to be loaded
		_plants = plants;*/

		archive.loadProperties(mycereal::make_nvp("name", name));
		archive.loadProperties(mycereal::make_nvp("plants",_plants.getInnerVector()));
		archive.loadProperties(mycereal::make_nvp("sprinklers", _sprinklers.getInnerVector()));
		archive.loadProperties(mycereal::make_nvp("programs",_programs.getInnerVector()));

		for(std::shared_ptr<Plant> plant : _plants.getInnerVector())
			plant->garden = this;
	//	correctPointers(); TODO:1111 decide how i keep my model correct!
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
