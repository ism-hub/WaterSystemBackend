/*
 * GardenAcceptable.h
 *
 *  Created on: 22 ����� 2017
 *      Author: IsM
 */

#ifndef MODEL_GARDENACCEPTABLE_H_
#define MODEL_GARDENACCEPTABLE_H_

namespace GardenModel {

class GardenAcceptable {
public:
	GardenAcceptable(){

	}
	virtual ~GardenAcceptable(){
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ GardenAcceptable DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	virtual void* accept(class GardenVisitor& visitor) = 0;
};

} /* namespace GardenModel */

#endif /* MODEL_GARDENACCEPTABLE_H_ */
