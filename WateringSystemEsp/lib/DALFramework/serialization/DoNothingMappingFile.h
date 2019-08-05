/*
 * DoNothingMappingFile.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#include <functional>
#include <vector>
#include <DALFramework/serialization/JsonContex.h>

#ifndef DALMODULE_SERIALIZATION_DONOTHINGMAPPINGFILE_H_
#define DALMODULE_SERIALIZATION_DONOTHINGMAPPINGFILE_H_

namespace mycereal {

class DoNothingMappingFile {
public:
	template<typename ... Args>
	void Model2Json(Args& ...) {}

	template<typename ... Args>
	void Json2Model(Args& ...) {}

	template<typename T, typename A>
	std::function<T* (std::vector<T, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<T, A>&, DAL::JsonContex&)
	{return std::function<T* (std::vector<T, A>&, DAL::JsonContex&)>();}
};

} /* namespace mycereal */

#endif /* DALMODULE_SERIALIZATION_DONOTHINGMAPPINGFILE_H_ */
