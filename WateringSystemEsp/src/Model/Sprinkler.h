/*
 * Sprinkler.h
 *
 *  Created on: 29 αιεπι 2017
 *      Author: IsM
 */

#ifndef SPRINKLER_H_
#define SPRINKLER_H_

//#include <DALModule/serialization/cereal2.h>
#include <WString.h>
#include <GardenAcceptable.h>
#include <GardenVisitor.h>
#include <JsonSerializationService2.h>

#include <ObserverDesignPattern/Property.hpp>


namespace GardenModel {
class Sprinkler : public GardenAcceptable {

public:
	Model::Property<int> id;
	enum Status { Off, On };
	//String name;
	Model::Property<Status> status;//we need to make sure that when we pulling the data from the flash the status is Off


	Sprinkler() : id(1), status(Off){
#ifdef DEBUG_MY_CODE
		Serial.println("Sprinkler CTOR");
#endif

		//id = 1;
		//name = "SprinklerNameee";
		status = Off;
	}

	/*Sprinkler(Sprinkler&& other){
		Serial.println("Sprinkler CPT_CTOR");
			id = other.id;
			//name = std::move(other.name);
			status = other.status;
		}*/

	virtual ~Sprinkler(){
#ifdef DEBUG_MY_CODE
		Serial.println("Sprinkler DTOR");
#endif

	}

	int getId(){
		return id;
	}
	void setId(int id){
		this->id = id;
	}

	/*String getName(){
		return name;
	}
	void setName(String name){
		this->name = name;
	}*/

	Status getStatus(){
		return status;
	}
	void setStatus(Status status){
		this->status = status;
	}

	bool operator==(Sprinkler const& rhs) const {
		return id == rhs.id;
	}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor){
		return visitor.visit(*this);
	}

	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		String status;
		status = (this->status == Off ? "Off" : "On");
		archive.addProperties(mycereal::make_nvp("id", id), mycereal::make_nvp("status", status));
	}

	template<class Archive>
	void load(Archive& archive) {
		String status;
		archive.loadProperties(mycereal::make_nvp("id", id), mycereal::make_nvp("status", status));
		this->status = (status == "Off" ? Off : On);
	}

};
}
#endif /* SPRINKLER_H_ */
