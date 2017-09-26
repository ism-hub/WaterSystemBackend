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
	int _id;
	String _name;
	Status _status;

	Sprinkler(){
		_id = 1;
		_name = "SprinklerNameee";
		_status = Off;
	}

	Sprinkler(Sprinkler&& other){
			_id = other._id;
			_name = std::move(other._name);
			_status = other._status;
		}

	virtual ~Sprinkler(){
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ Sprinkler DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	int getId(){
		return _id;
	}
	void setId(int id){
		_id = id;
	}

	String getName(){
		return _name;
	}
	void setName(String name){
		_name = name;
	}

	Status getStatus(){
		return _status;
	}
	void setStatus(Status status){
		_status = status;
	}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor){
		return visitor.visit(*this);
	}

	//im not sure if i want this thing in here, need to think about it.
	template <class Archive>
	void save(Archive& archive) const{
		archive(CEREAL2_NVP(_id));
	}

	template<class Archive>
		void load(Archive& archive) {
			archive(_id);
		}

};
}
#endif /* SPRINKLER_H_ */
