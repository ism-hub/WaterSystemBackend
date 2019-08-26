/*
 * StationRESTHsonVisitor.h
 *
 *  Created on: 1 ����� 2018
 *      Author: IsM
 */

#ifndef STATIONMODULE_REST_STATIONRESTJSONVISITOR_H_
#define STATIONMODULE_REST_STATIONRESTJSONVISITOR_H_

#include <HttpFramework.hpp>

#include <StationModule/REST/ListOfNetworks.h>
#include <StationModule/model/IStationVisitor.h>
#include <StationModule/REST/defs.h>

#include <StationModule/REST/StationRESTJsonVisitor.h>
#include <StationModule/REST/StationRESTSerializationService.h>

namespace sm {

class StationRESTJsonVisitor : public IStationVisitor, public Http::SerializationVisitor<StationRESTSerializationService, String> {
public:
	StationRESTJsonVisitor(std::shared_ptr<StationRESTSerializationService> serializationService) :
		Http::SerializationVisitor<StationRESTSerializationService, String>(serializationService){
//		Serial.println("-------------------- StationRESTJsonVisitor CTOR");
	}
	 ~StationRESTJsonVisitor() {
//		Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~ StationRESTJsonVisitor DTOR");
	}

	std::shared_ptr<void> visit(Station& station) {
		return Http::SerializationVisitor<StationRESTSerializationService, String>::visit(station);
	}
	std::shared_ptr<void> visit(WiFiNetwork& WifiNet) {
		return Http::SerializationVisitor<StationRESTSerializationService, String>::visit(WifiNet);
	}
	std::shared_ptr<void> visit(ConnectionStatus& connectionStatus) {
		return Http::SerializationVisitor<StationRESTSerializationService, String>::visit(connectionStatus);
	}
	std::shared_ptr<void> visit(ListOfNetworks& listOfNets) {
		return Http::SerializationVisitor<StationRESTSerializationService, String>::visit(listOfNets);
	}

};

} /* namespace sm */

#endif /* STATIONMODULE_REST_STATIONRESTJSONVISITOR_H_ */
