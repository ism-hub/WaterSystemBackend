/*
 * ModuleService.h
 *
 *  Created on: 10 בספט׳ 2017
 *      Author: IsM
 */

#ifndef MODULEFRAMEWORK_MODULESERVICE_H_
#define MODULEFRAMEWORK_MODULESERVICE_H_

//#include <typeinfo>
#include <map>
#include <ModuleFramework/Module.h>
#include <memory>
#include <ModuleFramework/Container/Container.h>

#include <ModuleFramework/utils.h>

//#include <type_traits>

namespace MF {

using StartFn = std::function<void()>;

class ModuleService {

public:
	int last_error_code = -1;

	std::shared_ptr<cntnr::Container> container = nullptr;

	//std::map<const std::type_info*,std::pair<std::shared_ptr<ModuleBase>,StartFn >> typeModuleMap;
	std::map<const void*, std::pair<std::shared_ptr<ModuleBase>,StartFn >> typeModuleMap;

	ModuleService(){
#ifdef DEBUG_MY_CODE
		Serial.println("ModuleService CTOR");
#endif
		container = std::make_shared<cntnr::Container>();
		container->registerInstance(container);
	}
	 ~ModuleService(){
#ifdef DEBUG_MY_CODE
		Serial.println("ModuleService DTOR");
#endif
		container->unregister<cntnr::Container>();
		}

	//creates module of the requested type and saves it to the map (registers it as one of the modules we need to call 'run/start' on)
	template<class ModuleType>
	std::shared_ptr<ModuleType> registerModule(){

		static_assert(std::is_base_of<ModuleBase, ModuleType>::value, "ModuleType must inherit from ModuleBase");


		std::shared_ptr<ModuleType> module = std::make_shared<ModuleType>();


		StartFn fn = [module, this]() {
#ifdef DEBUG_MY_CODE
			Serial.print("calling start member for module - ");
			Serial.println(MF::compiletimeTypeid<ModuleType>());
#endif
			this->container->memberFunctionResolver(*module, &ModuleType::start);//	module->start();
		};
		typeModuleMap[MF::compiletimeTypeid<ModuleType>()] = std::pair<std::shared_ptr<ModuleBase>,StartFn >(module, fn);

		return module;
	}


	bool startAllModules(){

		bool isValidConfiguration = isAlldependenciesRegistered() && !hasCircularDependency();

		if(!isValidConfiguration)
			return false;

		setAllColoredModulesToUncolored();

		for (const auto& kv : typeModuleMap) {
			startModuleButDependenciesFrist(kv.second);
		//	kv.second.second(); //calls each module 'start' function and inject the required dependencies.
		}

		return true;
	}

private:
	void setAllColoredModulesToUncolored() {
		for (const auto& kv : typeModuleMap) {
			kv.second.first->colored = false;
		}
	}

	//starts the module only if not colored already (colored == already started)
	void startModuleButDependenciesFrist(std::pair<std::shared_ptr<ModuleBase>,StartFn > moduleStarterPair){//TODO: I think we creating here way too many objects
		std::shared_ptr<ModuleBase> module = moduleStarterPair.first;
		if(module->colored)//if we already started this module
			return;

		for (const auto& typeinf : module->dependencies) {//run the dependencies first
			startModuleButDependenciesFrist(typeModuleMap[typeinf]);
		}


		moduleStarterPair.second();//calling the startFn
		module->colored = true;//mark that we started it
	}

protected:

	//checks if a module dependency is actually registered
	bool isAlldependenciesRegistered(){
		bool flag;
		for (const auto& kv : typeModuleMap) {
			flag = checkIfDependenciesAreRegistered(kv.second.first);
			if(!flag){//the module don't have some dependency
				//TODO: I need to handle error reports somehow (without exceptions) (which module has the problem? which dependency is missing?)
				Serial.print(F("_______CRITICAL ERROR_______: isAlldependenciesRegistered failed "));
				Serial.println((char*)kv.first);
				return false;
			}
		}
		return true;
	}

	//check there isn't any circular dependencies.
	//how to check that? there are many ways, i chose to - $$$$ I implemented something simpler than that...
	//	- every module is a node and his dependencies are his children (directed graph)
	//	- we start at each node and if he not colored we color him and proceeding to color his children
	//	- if we start at node that already colored we ignore it
	//	- if we start at node which not colored we and

	//  - the high of a node is the path with the maximum number of nodes possible
	//		(if we don't have cycles the maximum high can be - the number of modules minus one)
	//		(if we have cycle its infinity)
	//		(so if we have some high greater than number of modules minus one its high is infinity)
	//	- we will grade the high of every node and the moment we encounter node with size over number of modules minus one we will stop cause we have circular dependency
	//	- if we graded all the nodes hight and all under number of modules minus one we don't have circular dependency
	//	- complexity - we pass on each node only once so O(n)
	bool hasCircularDependency(){
		bool hasCycle;
		for (const auto& kv : typeModuleMap) {
			hasCycle = hasPathWithCycle(kv.second.first, 0);
			if (hasCycle){
				Serial.print(F("_______CRITICAL ERROR________: we have a circular dependency "));Serial.println((char*)kv.first);
				return true;
			}

		}
		return false;
	}

	//deptInTreeSoFar - how deep to the children we went from when we started
	bool hasPathWithCycle(std::shared_ptr<ModuleBase> module, int deptInTreeSoFar){
		if(deptInTreeSoFar > (int)typeModuleMap.size() - 1){ // we definitely have a cycle.
			return true;
		}
		else if(!module->colored){//not colored means we dont yet know if contains path with a cycle
			bool flag;
			for (const auto& typeinf : module->dependencies) {//foreach dependency we check if there is a path with cycle.
				flag = hasPathWithCycle(typeModuleMap[typeinf].first, deptInTreeSoFar + 1);
				if(flag){
					return true;
				}
			}
			module->colored = true;//no paths with cycles for this node (we just checked all its dependencies) so we color him.
			return false;
		}
		else //if(module->colored){//if it colored it means that we already checked that none of its paths contain cycles.
			return false;
	}

private:
	bool checkIfDependenciesAreRegistered(std::shared_ptr<ModuleBase> module){
		for (const auto& typeinf : module->dependencies) {
			if (typeModuleMap.find(typeinf) == typeModuleMap.end()) {
				return false;
			}
		}
		return true;
	}

};

} /* namespace moduleFramework */

#endif /* MODULEFRAMEWORK_MODULESERVICE_H_ */
