/*
 * SimpleProgram.h
 *
 *  Created on: 7 ����� 2017
 *      Author: IsM
 */

#ifndef MODEL_SIMPLEPROGRAM_H_
#define MODEL_SIMPLEPROGRAM_H_

#include <Model/TimePattern.h>
#include <GardenAcceptable.h>

namespace GardenModel {

class SimpleProgram : public GardenAcceptable{
public:

	Model::Property<int> id;
	Model::Property<String> name = String("not-set");
	Model::Property<TimePattern> timePattern;

	SimpleProgram() : id(0){
#ifdef DEBUG_MY_CODE
		Serial.println("SimpleProgram CTOR");
#endif
		}
	virtual ~SimpleProgram(){
#ifdef DEBUG_MY_CODE
		Serial.println("SimpleProgram DTOR");
#endif
		}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor) {
		return visitor.visit(*this);
	}

	template <class Archive>
	void save(Archive& archive) const{
		archive(CEREAL2_NVP(id), CEREAL2_NVP(name), CEREAL2_NVP(timePattern));
	}

	template<class Archive>
	void load(Archive& archive) {
		archive(id, name, timePattern);
	}

};

} /* namespace DAL */

#endif /* MODEL_SIMPLEPROGRAM_H_ */