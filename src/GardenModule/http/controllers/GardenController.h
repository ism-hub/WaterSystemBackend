/*
 * GardenController.h
 *
 *  Created on: 27 ����� 2018
 *      Author: rina
 */

#ifndef HTTPMODULE_CONTROLLERS_GARDENCONTROLLER_H_
#define HTTPMODULE_CONTROLLERS_GARDENCONTROLLER_H_

#include <GardenModule/DAL/repositoryPattern/GardenUnitOfWork.h>
#include <HttpFramework.hpp>
#include <memory>
#include <GardenModule/model/GardenAcceptable.h>
#include <GardenModule/model/Garden.h>

#include <GardenModule/http/jsonSerialization/GardenRESTSerializationService.h>

namespace garden {

class GardenController: public Http::IController<garden::GardenAcceptable, String> {

protected:
	std::shared_ptr<GardenUnitOfWork> _unitOfWork;
	std::shared_ptr<garden::GardenRESTSerializationService> _apiSerService;
public:
	GardenController(std::shared_ptr<GardenUnitOfWork> unitOfWork,
			std::shared_ptr<garden::GardenRESTSerializationService> apiSerService):
				_unitOfWork(unitOfWork), _apiSerService(apiSerService) {}
	 ~GardenController() {}

	bool canHandle(Http::HttpServletRequest<String>& req) {
		if(req.httPMethod == Http::HTTPMethod::HTTP_GET && req.urlTokens.size() == 0){
			return true;
		}
		if(req.httPMethod ==  Http::HTTPMethod::HTTP_POST && req.urlTokens.size() == 0){
			return true;
		}
		return false;
	}

	std::shared_ptr<GardenAcceptable> handle(Http::HttpServletRequest<String>& req, Http::HttpServletResponse<String>&) {
		std::shared_ptr<Garden> garden = nullptr;
		if(canHandle(req) && req.httPMethod ==  Http::HTTPMethod::HTTP_GET){
			garden = _unitOfWork->Garden().getById(-1);
		}
		if(canHandle(req) && req.httPMethod == Http::HTTPMethod::HTTP_POST && req.requestBody != ""){
			garden = _unitOfWork->Garden().getById(-1);
			_apiSerService->Json2Model(*garden, req.requestBody);
			if(!_unitOfWork->complete())
				Serial.println(F("__ERR didnt save the garden "));
		}
		return garden;
	}

};

} /* namespace Http */

#endif /* HTTPMODULE_CONTROLLERS_GARDENCONTROLLER_H_ */
