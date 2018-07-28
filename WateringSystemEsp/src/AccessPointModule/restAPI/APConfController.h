/*
 * GardenController.h
 *
 *  Created on: 27 áôáø× 2018
 *      Author: rina
 */

#ifndef HTTPMODULE_CONTROLLERS_GARDENCONTROLLER_H_
#define HTTPMODULE_CONTROLLERS_GARDENCONTROLLER_H_

#include <DALModule/repositoryPattern/GardenUnitOfWork.h>
#include <httpModule/dispatcher/Controller.h>
#include <httpModule/model/HttpServletRequest.h>
#include <httpModule/model/HttpServletResponse.h>
#include <memory>
#include <GardenAcceptable.h>
#include <Garden.h>

namespace Http {

class APConfController: public Controller {
	typedef DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::APIMappingFile>,mycereal::JsonLoadArchive<DAL::APIMappingFile>> APISerializationSerice;
protected:
	std::shared_ptr<DAL::GardenUnitOfWork> _unitOfWork;
	std::shared_ptr<APISerializationSerice> _apiSerService;
public:
	GardenController(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork,
			std::shared_ptr<DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::APIMappingFile>,mycereal::JsonLoadArchive<DAL::APIMappingFile>>> apiSerService):
				_unitOfWork(unitOfWork), _apiSerService(apiSerService) {}
	virtual ~GardenController() {}

	bool canHandle(HttpServletRequest& req) {
		if(req.httPMethod == HTTP_GET && req.urlTokens.size() == 0){
			return true;
		}
		if(req.httPMethod == HTTP_POST && req.urlTokens.size() == 0){
			return true;
		}
		return false;
	}

	std::shared_ptr<GardenAcceptable> handle(HttpServletRequest& req, HttpServletResponse&) {
		std::shared_ptr<Garden> garden = nullptr;
		if(canHandle(req) && req.httPMethod == HTTP_GET){
			garden = _unitOfWork->Garden().getById(-1);
		}
		if(canHandle(req) && req.httPMethod == HTTP_POST && req.requestBody != ""){
			Serial.println("inside GardenController 'handle' handling post");
			garden = _unitOfWork->Garden().getById(-1);
			_apiSerService->Json2Model(*garden, req.requestBody);
			Serial.println(req.requestBody);
			Serial.println("QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ the durations we got from the GUI merged into our model");
			Serial.println(garden->_programs.getInnerVector()[0].get()->timePattern.get().days[0].hours[0].duration);
			Serial.println(garden->_programs.getInnerVector()[1].get()->timePattern.get().days[0].hours[0].duration);
			if(!_unitOfWork->complete())
				Serial.println("__ERR didnt save the garden ");
		}
		return garden;
	}

};

} /* namespace Http */

#endif /* HTTPMODULE_CONTROLLERS_GARDENCONTROLLER_H_ */
