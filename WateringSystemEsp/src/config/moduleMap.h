/*
 * moduleMap.h
 *
 *  Created on: 22 ����� 2017
 *      Author: IsM
 */

#ifndef CONFIG_MODULEMAP_H_
#define CONFIG_MODULEMAP_H_

#include <ModuleFramework/ModuleService.h>
#include <DALModule/DALModule.h>

#include <httpModule/httpModule.h>

#include <ServiceFrameworkModule/ServiceFrameworkModule.h>

#include <SchedulerModule/SchedulerModule.h>
#include <HardwareModule/HardwareModule.h>
#include <TimeModule/TimeModule.h>

#include <AccessPointModule/AccessPointModule.h>

#include <StationModule/StationModule.h>

namespace config {

void moduleMap(MF::ModuleService& mfs) {

	mfs.registerModule<tsm::TimeModule>()->
			registerDependenciesTypes<DALModule::DALModule,
								  	  sfwkModule::ServiceFrameworkModule>();

	mfs.registerModule<sfwkModule::ServiceFrameworkModule>();

	mfs.registerModule<schedmodule::SchedulerModule>()->
			registerDependenciesTypes<tsm::TimeModule>();

	mfs.registerModule<DALModule::DALModule>();

	mfs.registerModule<hrdwrmodule::HardwareModule>()->
			registerDependenciesTypes<schedmodule::SchedulerModule,
										sfwkModule::ServiceFrameworkModule,
										DALModule::DALModule>();

	mfs.registerModule<httpModule::httpModule>()->
			registerDependenciesTypes<DALModule::DALModule>();

	mfs.registerModule<apm::AccessPointModule>()->
			registerDependenciesTypes<schedmodule::SchedulerModule,
										sfwkModule::ServiceFrameworkModule,
										httpModule::httpModule,
										DALModule::DALModule>();

	mfs.registerModule<sm::StationModule>()->
			registerDependenciesTypes<schedmodule::SchedulerModule,
										sfwkModule::ServiceFrameworkModule,
										httpModule::httpModule,
										DALModule::DALModule>();
}

} /* namespace moduleFramework */

#endif /* CONFIG_MODULEMAP_H_ */
