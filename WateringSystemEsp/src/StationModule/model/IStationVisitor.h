/*
 * IStationModeVisitor.h
 *
 *  Created on: 30 αιεμι 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_MODEL_ISTATIONVISITOR_H_
#define STATIONMODULE_MODEL_ISTATIONVISITOR_H_

#include <memory>

//#include <StationModule/model/Station.h>
//#include <StationModule/model/WiFiNetwork.h>
//#include <StationModule/model/ConnectionStatus.h>

namespace sm {
class IStationAcceptable;
class Station;
class WiFiNetwork;
class ConnectionStatus;
class ListOfNetworks;

class IStationVisitor {

public:
	IStationVisitor() {}
	virtual ~IStationVisitor() {}

	virtual std::shared_ptr<void> visit(Station& stationAcceptable) = 0;
	virtual std::shared_ptr<void> visit(WiFiNetwork& WifiNet) = 0;
	virtual std::shared_ptr<void> visit(ConnectionStatus& connectionStatus) = 0;
	virtual std::shared_ptr<void> visit(ListOfNetworks& listOfNetworks) = 0;
};

} /* namespace sm */

#endif /* STATIONMODULE_MODEL_ISTATIONVISITOR_H_ */
