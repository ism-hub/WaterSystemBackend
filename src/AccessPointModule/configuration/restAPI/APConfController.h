/*
 * GardenController.h
 *
 *  Created on: 27 áôáø× 2018
 *      Author: rina
 */

#ifndef ACCESSPOINTMODULE_RESTAPI_APCONFCONTROLLER_H_
#define ACCESSPOINTMODULE_RESTAPI_APCONFCONTROLLER_H_

#include <HttpFramework/dispatcher/IController.h>
#include <HttpFramework/model/HttpServletRequest.h>
#include <HttpFramework/model/HttpServletResponse.h>
#include <memory>

#include <AccessPointModule/configuration/model/IAPConfAcceptable.h>
#include <AccessPointModule/configuration/DAL/APConfContex.h>

#include <DALFramework/serializationService/DefaultSerializationService.h>

namespace apm {

class APConfController: public Http::IController<IAPConfAcceptable> {

protected:
	std::shared_ptr<APConfContex> _contex;
	std::shared_ptr<DALModule::DefaultSerializationService> _serializationService;
	std::shared_ptr<APService> apService;
public:
	APConfController(std::shared_ptr<APConfContex> contex,
			std::shared_ptr<DALModule::DefaultSerializationService> serializationService,
			std::shared_ptr<APService> apService):
				_contex(contex), _serializationService(serializationService),  apService(apService) {}
	virtual ~APConfController() {}

	bool canHandle(Http::HttpServletRequest& req) {
	//	Serial.println("inside APConfController 'canHandle' handling post");
		bool flg = false;
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() > 0 && req.urlTokens[0] == _contex->get()->apConfigRestAPIPath){
			flg = true;
		}
		if(req.httPMethod == Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 0){
			flg = true;
		}
	//	Serial.println("End inside APConfController 'canHandle' handling post");
		return flg;
	}

	std::shared_ptr<IAPConfAcceptable> handle(Http::HttpServletRequest& req, Http::HttpServletResponse&) {
		std::shared_ptr<APConfiguration> model = nullptr;
		if(canHandle(req) && req.httPMethod == Http::HTTPMethod::HTTP_GET){
			model = std::make_shared<APConfiguration>(apService->getAPConfiguration());
		}
		if(canHandle(req) && req.httPMethod == Http::HTTPMethod::HTTP_POST && req.requestBody != ""){
		//	Serial.println("inside APConfController 'handle' handling post");
			model = _contex->get();
			_serializationService->Json2Model(*model, req.requestBody);
		//	if(!_contex->save())
		//		Serial.println("__ERR didn't save the APConfiguration ");
			apService->setAPConfiguration(*model);
		}
		return model;
	}

};

} /* namespace apm */

#endif /* ACCESSPOINTMODULE_RESTAPI_APCONFCONTROLLER_H_ */
