/*
 * ServiceFrameworkModule.h
 *
 *  Created on: 5 במרץ 2018
 *      Author: IsM
 */

#ifndef SERVICEFRAMEWORKMODULE_SERVICEFRAMEWORKMODULE_H_
#define SERVICEFRAMEWORKMODULE_SERVICEFRAMEWORKMODULE_H_

#include <memory>
#include <ModuleFramework/Module.h>
#include <ModuleFramework/Container/Container.h>

#include <ServiceFramework/ServiceFrameWork.h>

namespace sfwkModule {

std::shared_ptr<sfwk::ServiceFrameWork> ServiceFrameWorkCreator( ){
	return std::make_shared<sfwk::ServiceFrameWork>();
}


class ServiceFrameworkModule : public MF::ModuleBase {
public:
	ServiceFrameworkModule() {}
	~ServiceFrameworkModule() {}

	void start(std::shared_ptr<cntnr::Container> container){
//		Serial.println("Inside ServiceFrameworkModule start ");
		container->registerType<sfwk::ServiceFrameWork>(&ServiceFrameWorkCreator);//always gives the same unit of work (meaning all the program share the same context model)
//		Serial.println("End Inside ServiceFrameworkModule start ");
	}
};

} /* namespace sfwkModule */

#endif /* SERVICEFRAMEWORKMODULE_SERVICEFRAMEWORKMODULE_H_ */
