/*
 * GardenController.h
 *
 *  Created on: 27 ����� 2018
 *      Author: rina
 */

#ifndef ACCESSPOINTMODULE_RESTAPI_APSTATUSCONTROLLER_H_
#define ACCESSPOINTMODULE_RESTAPI_APSTATUSCONTROLLER_H_

#include <HttpFramework.hpp>
#include <memory>

#include <AccessPointModule/status/model/IAPStatusAcceptable.h>
#include <AccessPointModule/status/model/APStatus.h>

namespace apm {

class APStatusController: public Http::IController<IAPStatusAcceptable, String> {

protected:
	std::shared_ptr<APService> apService;
	String urlPath;
public:
	APStatusController(std::shared_ptr<APService> apService, String urlPath = "apstatus")://TODO: add that string to some config file
		apService(apService), urlPath(urlPath) {}
	virtual ~APStatusController() {}

	bool canHandle(Http::HttpServletRequest<String>& req) {
	//	Serial.println("inside APStatusController 'canHandle' handling post");
		bool flg = false;
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() > 0 && req.urlTokens[0] == urlPath){
			flg = true;
		}
		return flg;
	}

	std::shared_ptr<IAPStatusAcceptable> handle(Http::HttpServletRequest<String>& req, Http::HttpServletResponse<String>&) {
		if(canHandle(req) && req.httPMethod == Http::HTTPMethod::HTTP_GET){
			return make_shared<APStatus>(apService->getAPStatus());
		}
		return nullptr;
	}

};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_RESTAPI_APSTATUSCONTROLLER_H_ */