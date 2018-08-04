/*
 * GardenVisitor.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef MODEL_GARDENVISITOR_H_
#define MODEL_GARDENVISITOR_H_



namespace garden {

class GardenVisitor {
public:
	GardenVisitor(){
#ifdef DEBUG_MY_CODE
		Serial.println("GardenVisitor CTOR");
#endif

	}
	virtual ~GardenVisitor(){
#ifdef DEBUG_MY_CODE
		Serial.println("GardenVisitor DTOR");
#endif
	}

	virtual std::shared_ptr<void> visit(class Garden& garden) = 0;

	virtual std::shared_ptr<void> visit(class Plant& plant) = 0;

	virtual std::shared_ptr<void> visit(class Sprinkler& sprinkler) = 0;

	virtual std::shared_ptr<void> visit(class SimpleProgram& program) = 0;
};

} /* namespace GardenModel */

#endif /* MODEL_GARDENVISITOR_H_ */
