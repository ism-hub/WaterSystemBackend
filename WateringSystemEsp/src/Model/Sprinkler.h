/*
 * Sprinkler.h
 *
 *  Created on: 29 αιεπι 2017
 *      Author: IsM
 */

#ifndef SPRINKLER_H_
#define SPRINKLER_H_

#include <DALModule/serialization/cereal2.h>
#include <WString.h>
#include <GardenAcceptable.h>
#include <GardenVisitor.h>


namespace GardenModel {
class Sprinkler : public GardenAcceptable {

public:
	enum Status { On, Off };
	int id;
	//String name;
	Status status;

	Sprinkler(){
		Serial.println("Sprinkler CTOR");
		id = 1;
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
		Serial.println("Sprinkler DTOR");
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

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor){
		return visitor.visit(*this);
	}

	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		archive(CEREAL2_NVP(id));
	}

	template<class Archive>
		void load(Archive& archive) {
			archive(id);
		}

};
}
#endif /* SPRINKLER_H_ */
