/*
 * Container.h
 *
 *  Created on: 13 בספט׳ 2017
 *      Author: IsM
 */

#ifndef CONTAINER_CONTAINER_H_
#define CONTAINER_CONTAINER_H_

#include <vector>
#include <memory>
//#include <typeinfo>
#include <map>
#include <utility>
#include <ModuleFramework/utils.h>

namespace cntnr {

using namespace MF;// deleteee

class Container {

	template<class InstanceType, class Ret, class ...Deps>
	using class_member_function = Ret(InstanceType::*)(std::shared_ptr<Deps> ...);

	template<class Ret ,class ...Deps>
	using ctor_function = Ret(*)(Deps ...);

	//class RegisteredType;
	//using CreateFn = std::function<std::shared_ptr<void>(RegisteredType&)>;
public:

	class RegisteredType {
	public:
		using CreateFn = std::function<std::shared_ptr<void>(RegisteredType&)>;
	private:
		CreateFn _create;
	public:
		//const std::type_info* _typeinfo;
		const void* _compiletimeType;
		bool isSingelton;
		std::shared_ptr<void> obj = nullptr;

		RegisteredType(const void* typeinfo, CreateFn createFn, bool isSingelton) :
			_create(createFn), _compiletimeType(typeinfo), isSingelton(isSingelton)  {
		}

		std::shared_ptr<void> create(){
			return _create(*this);
		}

	/*	RegisteredType(RegisteredType&& other){
			_typeinfo = other._typeinfo;
			_create = std::move(other._create);
			isSingelton = other.isSingelton;
			other._typeinfo = NULL;
			//other._create = ???
		}*/

		virtual ~RegisteredType() {}

		template<class T>
		RegisteredType& as() {
			_compiletimeType = MF::compiletimeTypeid<T>();
			return *this;
		}

		//RegisteredType& alwaysNew() {
		//	isSingelton = false;
		//	return *this;
		//}

		std::shared_ptr<void> getInstance() {
			return _create(*this);
		}

	};

	std::map<const void*, std::vector<RegisteredType> > typeRegisteredTypeMap;

	Container(){Serial.println("Container CTOR");}
	virtual ~Container(){Serial.println("Container DTOR");}

	template <class T>
	void registerInstance(std::shared_ptr<T> instance){
		const void* compileType = MF::compiletimeTypeid<T>();

		RegisteredType::CreateFn create = [](RegisteredType& regType) {
					return regType.obj;
				};

		RegisteredType regType(compileType, create, true);
		regType.obj = instance;
		typeRegisteredTypeMap[compileType].push_back(std::move(regType));
	}

	template<class T>
	void registerType(bool isSingleton = true) {
		ctor_function<std::shared_ptr<T>> createFnc = [](){return std::make_shared<T>();};
		registerType<T>(createFnc, isSingleton);
	}

	template< class Ret, class ...Deps >
	void registerType(ctor_function<std::shared_ptr<Ret>, Deps ...> ctorFnc, bool isSingleton = true) {
		const void* compileType = MF::compiletimeTypeid<Ret>();
		RegisteredType::CreateFn create = [&, ctorFnc](RegisteredType& regType) {
			Serial.print( "container registerType createFnc ");
			Serial.println( MF::compiletimeTypeid<Ret>());
			std::shared_ptr<Ret> obj = std::static_pointer_cast<Ret>(regType.obj);
			if(regType.obj == nullptr) {
				 obj = this->ctorFunctionResolver(ctorFnc);
				//obj = this->resolveInner<T>();
				if(regType.isSingelton)
					regType.obj = obj;
			}
			return obj;
		};

		//RegisteredType regType(compileType, create, isSingleton);
//TODO: check what happening here

		typeRegisteredTypeMap[compileType].push_back(RegisteredType(compileType, create, isSingleton));//adding to the .text @@@@
	//	if(typeRegisteredTypeMap.find(compileType) == typeRegisteredTypeMap.end()){//no such type;
	//		//typeRegisteredTypeMap.insert(std::pair<const void*, std::vector<RegisteredType> >(compileType, std::vector<RegisteredType>{std::move(regType)}));
	//		typeRegisteredTypeMap[compileType].push_back(std::move(regType)); //= std::vector<RegisteredType>{std::move(regType)};
	//	}
	//	else{
	//		typeRegisteredTypeMap[compileType].push_back(std::move(regType));

	//	}
	}


	template<class T>
	std::vector<std::shared_ptr<T>> resolveAll() {
		std::vector<std::shared_ptr<T>> t_vector;
		if (typeRegisteredTypeMap.find(MF::compiletimeTypeid<T>())== typeRegisteredTypeMap.end())	//we didnt register it
			return t_vector;//empty vector

		for (auto &t : typeRegisteredTypeMap[MF::compiletimeTypeid<T>()]) // access by reference to avoid copying
		{
			t_vector.push_back(std::static_pointer_cast<T>(t.create()));
		}
		return t_vector;
	}

private:
	template <typename T, typename _ = void>
	struct is_vector {
	    static const bool value = false;
	};
	template <typename T>
	struct is_vector< T, typename enable_if<is_same<T, std::vector< typename T::value_type, typename T::allocator_type > >::value>::type>
	{
	    static const bool value = true;
	};

	template <typename T, typename _ = void>
		struct is_shared_ptr {
		    static const bool value = false;
		};
		template <typename T>
		struct is_shared_ptr< T, typename enable_if<is_same<T, std::shared_ptr<typename T::element_type> >::value>::type>
		{
		    static const bool value = true;
		};


	enum class sfinae {};

	template<class T, typename std::enable_if<is_shared_ptr<T>::value, sfinae>::type = { }>
	T resolve() {
		return resolve<typename T::element_type>();
	}

	template<class T,typename std::enable_if<is_vector<T>::value, sfinae>::type = { }>
	T resolve() {
		return resolveAll<typename T::value_type::element_type >();
	}

public:
	template<class T, typename std::enable_if<(!is_vector<T>::value && !is_shared_ptr<T>::value), sfinae>::type ={ }>
	std::shared_ptr<T> resolve() {
		//check if we have that type registered
		if (typeRegisteredTypeMap.find(MF::compiletimeTypeid<T>())== typeRegisteredTypeMap.end()) //we didnt register it
			return nullptr;
		return std::static_pointer_cast<T>(typeRegisteredTypeMap[MF::compiletimeTypeid<T>()].front().create());
	}

	template <class T>
	void unregister(){
		typeRegisteredTypeMap.erase(MF::compiletimeTypeid<T>());//adding to the .text @@@@
	}

	template<class InstanceType, class Ret, class ...Deps>
	void memberFunctionResolver(InstanceType& instance, class_member_function<InstanceType, Ret, Deps...> ctorFnc){
		//std::cout << typeid(InstanceType).name() << std::endl;
		//using expander = int[];
		//(void)expander{0, ((void)(std::cout << typeid(Deps).name() << std::endl), 0)...};

		(instance.*ctorFnc)( resolve<Deps>() ...);
	}

	template <class Ret, class ...Deps>
	std::shared_ptr<Ret> ctorFunctionResolver(ctor_function< std::shared_ptr<Ret>, Deps ...> ctorFunction){
		return ctorFunction(resolve<Deps>() ...);
	}

protected:
/*	template<class T>
	std::shared_ptr<T> defaultCreator(){
		std::shared_ptr<T> obj = std::make_shared<T>();
		this->memberFunctionResolver(*obj ,&T::start);
		return obj;
	}*/

};

} /* namespace moduleFramework */

#endif /* CONTAINER_CONTAINER_H_ */
