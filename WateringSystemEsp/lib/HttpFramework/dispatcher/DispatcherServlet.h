/*
 * DispatcherServlet.h
 *
 *  Created on: 10 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_DISPATCHERSERVLET_H_
#define HTTP_DISPATCHERSERVLET_H_

#include <memory>
#include <GardenModule/http/controllers/PlantController.h>
#include <GardenModule/http/controllers/PlantController.h>

#include <HttpFramework/dispatcher/IHandlerExecutionChain.h>
#include <GardenModule/http/jsonSerialization/JsonHandlerInterceptor.h>
#include <HttpFramework/model/HttpServletRequest.h>
#include <HttpFramework/model/HttpServletResponse.h>
#include <vector>



using namespace std;
using namespace garden;

namespace Http {

class DispatcherServlet {
	std::shared_ptr<IHandlerExecutionChain> invalidChain = nullptr;
public:
	vector<std::shared_ptr<IHandlerExecutionChain>> _handlerExecutionChains;

	DispatcherServlet(vector<std::shared_ptr<IHandlerExecutionChain>> exeChains = {}) : _handlerExecutionChains(exeChains) {
	}

	 ~DispatcherServlet() {
	}

	std::shared_ptr<IHandlerExecutionChain>& getHandlerExecutionChain(HttpServletRequest& request) {
		for(auto& exeChain : _handlerExecutionChains){
			if(exeChain->canHandle(request))
				return exeChain;
		}
		return invalidChain;
	}

	std::shared_ptr<HttpServletResponse> dispatch(HttpServletRequest& request) {
		std::shared_ptr<IHandlerExecutionChain>& executionChain = getHandlerExecutionChain(request);
		if(executionChain == nullptr) {
			std::shared_ptr<HttpServletResponse> resp = std::make_shared<HttpServletResponse>();
			resp->_httpCode = SC_NOT_FOUND;
			return resp;
		}
		std::shared_ptr<HttpServletResponse> response = executionChain->execute(request);
		return response;
	}

};

} /* namespace Http */

#endif /* HTTP_DISPATCHERSERVLET_H_ */
