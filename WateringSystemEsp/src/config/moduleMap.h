/*
 * moduleMap.h
 *
 *  Created on: 22 בספט׳ 2017
 *      Author: IsM
 */

#ifndef CONFIG_MODULEMAP_H_
#define CONFIG_MODULEMAP_H_

#include <ModuleFramework/ModuleService.h>
#include <DALModule/DALModule.h>

#include <httpModule/httpModule.h>

namespace config {

void moduleMap(MF::ModuleService& mfs){
	mfs.registerModule<DALModule::DALModule>();

	mfs.registerModule<httpModule::httpModule>()->
			registerDependenciesTypes<DALModule::DALModule>();
}

} /* namespace moduleFramework */

#endif /* CONFIG_MODULEMAP_H_ */
