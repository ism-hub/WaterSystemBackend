/*
 * Sprinkler.h
 *
 *  Created on: 29 αιεπι 2017
 *      Author: IsM
 */

#ifndef SPRINKLER_H_
#define SPRINKLER_H_

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

	virtual void* accept(GardenVisitor& visitor){
		return visitor.visit(*this);
	}

};
}
#endif /* SPRINKLER_H_ */
