/*
 * IStationAcceptable.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_MODEL_ISTATIONACCEPTABLE_H_
#define STATIONMODULE_MODEL_ISTATIONACCEPTABLE_H_

#include <StationModule/model/IStationVisitor.h>
#include <memory>


namespace sm {

class IStationAcceptable {
public:
	IStationAcceptable() {}
	virtual ~IStationAcceptable() {}

	virtual std::shared_ptr<void> accept(IStationVisitor& visitor) = 0;
};

} /* namespace sm */

#endif /* STATIONMODULE_MODEL_ISTATIONACCEPTABLE_H_ */
