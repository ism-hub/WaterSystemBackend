/*
 * DoNothingMappingFile.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#ifndef DALMODULE_SERIALIZATION_DONOTHINGMAPPINGFILE_H_
#define DALMODULE_SERIALIZATION_DONOTHINGMAPPINGFILE_H_

namespace mycereal {

class DoNothingMappingFile {
public:
	template<typename ... Args>
	void Model2Json(Args& ...) {}

	template<typename ... Args>
	void Json2Model(Args& ...) {}
};

} /* namespace mycereal */

#endif /* DALMODULE_SERIALIZATION_DONOTHINGMAPPINGFILE_H_ */
