/*
 * ServiceFrameWork.h
 *
 *  Created on: 5 במרץ 2018
 *      Author: IsM
 */

#ifndef SERVICEFRAMEWORK_SERVICEFRAMEWORK_H_
#define SERVICEFRAMEWORK_SERVICEFRAMEWORK_H_

#include <vector>
#include <memory>
#include <ServiceFramework/IService.h>

namespace sfwk {

// Not really a framework (using the module framework to load the services in the correct order and for the dependency injection)
// Putting the service here makes sure that the service will stay alive for the runtime of the program (for now that is the only reason i need this model).
// Beside the above, if we want to manage the services (restart, delete etc (i don't need those use cases so i wont implement them)) it will be here
class ServiceFrameWork {
	std::vector<std::shared_ptr<IService>> _services;
public:
	ServiceFrameWork() {}
	 ~ServiceFrameWork() {}

	int AddService(std::shared_ptr<IService> service){
		_services.push_back(service);
		return 1;
	}
};

} /* namespace sfwk */

#endif /* SERVICEFRAMEWORK_SERVICEFRAMEWORK_H_ */
