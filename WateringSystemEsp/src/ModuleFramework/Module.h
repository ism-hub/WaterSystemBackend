/*
 * Module.h
 *
 *  Created on: 11 בספט׳ 2017
 *      Author: IsM
 */

#ifndef MODULEFRAMEWORK_MODULE_H_
#define MODULEFRAMEWORK_MODULE_H_

#include <memory>
#include <vector>
//#include <typeinfo>
#include <utility>
#include <iterator>
#include <ModuleFramework/utils.h>

namespace MF {

class ModuleBase {
	 friend class ModuleService;
private:
	bool colored = false;
protected:

public:
	std::vector<const void*> dependencies;

	ModuleBase(){}
	virtual ~ModuleBase(){}

	template <class... Dependencies>
	void registerDependenciesTypes(){

		const std::vector<const void*> mem = { compiletimeTypeid<Dependencies>() ...};
		//a.insert(std::end(a), std::begin(b), std::end(b));
		dependencies.insert(std::end(dependencies), std::begin(mem), std::end(mem));
	}

};

//template member function can't be virtual,
// and I need a function that can take multiple different variables
//(I need to force the client to implement it, its his entry point, there isn't any sense to give it default impl)
template <class DerivedType>
class Module: public ModuleBase{
public:
	DerivedType* _derived;

	Module(DerivedType* derived){
		_derived = derived;
	}
	virtual ~Module(){}

	template<class... Args>
	void start(Args... args){
		_derived->start(std::forward<Args>(args)...);
	}

};




} /* namespace moduleFramework */

#endif /* MODULEFRAMEWORK_MODULE_H_ */
