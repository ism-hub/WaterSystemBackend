/*
 * SimpleProgram.h
 *
 *  Created on: 7 срхїз 2017
 *      Author: IsM
 */

#ifndef MODEL_SIMPLEPROGRAM_H_
#define MODEL_SIMPLEPROGRAM_H_

#include <GardenModule/model/TimePattern.h>
#include <GardenModule/model/GardenAcceptable.h>
#include <DALFramework/serialization/NameValuePair.h>

namespace garden {

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
	 ~SimpleProgram(){
#ifdef DEBUG_MY_CODE
		Serial.println("SimpleProgram DTOR");
#endif
		}

	virtual std::shared_ptr<void> accept(GardenVisitor& visitor) {
		return visitor.visit(*this);
	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const{
		archive.addProperties(mappingFile, MACRO_NVP(id), MACRO_NVP(name), MACRO_NVP(timePattern));
	}

	template<class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
		archive.loadProperties(mappingFile, MACRO_NVP(id), MACRO_NVP(name), MACRO_NVP(timePattern));
	}

};

} /* namespace DAL */

#endif /* MODEL_SIMPLEPROGRAM_H_ */
