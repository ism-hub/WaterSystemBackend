/*
 * StationFlashMappingFile.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_DAL_STATIONFLASHMAPPINGFILE_H_
#define STATIONMODULE_DAL_STATIONFLASHMAPPINGFILE_H_

#include <functional>
#include <vector>
#include <DALFramework/serialization/JsonContex.h>

#include <StationModule/model/Station.h>

namespace sm {

class StationFlashMappingFile {
public:
	StationFlashMappingFile() {}
	 ~StationFlashMappingFile() {}

	template<typename ... Args>
	void Model2Json(Args& ...) {}

	template<typename ArchiveType>
	void Model2Json(const Station& , ArchiveType& archive) {
		//we don't want to serialize the station station.connectionStatus
		DAL::JsonContex& context = archive.getCurrentContex();
		context.listOfNotAllowedKeys.push_back(F("connectionStatus"));
	}

	template<typename ... Args>
	void Json2Model(Args& ...) {}

	template<typename T, typename A>
	std::function<T* (std::vector<T, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<T, A>&, DAL::JsonContex&)
	{return std::function<T* (std::vector<T, A>&, DAL::JsonContex&)>();}
};

} /* namespace sm */

#endif /* STATIONMODULE_DAL_STATIONFLASHMAPPINGFILE_H_ */
