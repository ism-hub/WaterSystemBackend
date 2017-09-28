/*
 * SomeModule.h
 *
 *  Created on: 21 בספט׳ 2017
 *      Author: IsM
 */

#ifndef MODULEFRAMEWORK_SOMEMODULE_H_
#define MODULEFRAMEWORK_SOMEMODULE_H_

#include <ModuleFramework/Module.h>
#include <ModuleFramework/utils.h>
#include <memory>
#include <ModuleFramework/Container/Container.h>

namespace MF {

class AA{

public:
	AA(){
		Serial.println("CTOR AA");
	}

	virtual ~AA(){Serial.println("DTOR AA");}
};

std::shared_ptr<AA> AACtorFnc(){
	Serial.println("AACtorFnc()");
	return std::make_shared<AA>();
}

class ModuleA: public MF::ModuleBase {
public:
	ModuleA() {Serial.println("ModuleA CTOR"); }
	virtual ~ModuleA(){Serial.println("ModuleA DTOR");}

	void start(std::shared_ptr<cntnr::Container> ctr){
		Serial.println("ModuleA::start()");
		ctr->registerType<AA>();//(&AACtorFnc);
	}
};

class SomeModule: public MF::ModuleBase {
public:
	SomeModule() {Serial.println("someModule CTOR"); }
	virtual ~SomeModule(){Serial.println("someModule DTOR");}

	std::shared_ptr<AA> _aa;

	void start(std::shared_ptr<AA> aa){
		Serial.println("someModule::start(AA aa)");
		_aa = aa;
	}
};

} /* namespace moduleFramework */

#endif /* MODULEFRAMEWORK_SOMEMODULE_H_ */
