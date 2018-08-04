/*
 * GardenRESTMappingFile.h
 *
 *  Created on: 3 баев„ 2018
 *      Author: IsM
 */

#ifndef GARDENMODULE_DAL_GARDENRESTMAPPINGFILE_H_
#define GARDENMODULE_DAL_GARDENRESTMAPPINGFILE_H_

#include <vector>
#include <algorithm>
#include <WString.h>
#include <StringUtils.h>
#include <DALFramework/serialization/NameValuePair.h>
#include <GardenModule/model/Garden.h>
#include <DALFramework/serialization/JsonContex.h>
#include <memory>
#include <ObserverDesignPattern/Property.hpp>
#include <stuff/stuff.h>

namespace garden {

class GardenRESTMappingFile {
public:
	template<typename ... Args>
	void Model2Json(Args& ...) {}

	template<typename ... Args>
	void Json2Model(Args& ...) {}

	template<typename T, typename A>
	std::function<T* (std::vector<T, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<T, A>&, DAL::JsonContex&)
	{return std::function<T* (std::vector<T, A>&, DAL::JsonContex&)>();}

	template<typename ArchiveType>
	void Model2Json(const garden::Plant& plant,  ArchiveType& archive) {//we want the program and sprinkler keys to be the id instead of the whole object
		String key;
		std::vector<stf::Link> links;
		DAL::JsonContex& context = archive.getCurrentContex();

		key = F("program");
		if(plant._program() != nullptr){// if the plant have the program, adding a key with its id
			context.nextName = &key;
			const int& pid = plant._program()->id();
			char cpid[5];
			itoa(pid, cpid, 10);// String(pid);
			String id(cpid);
			links.push_back(stf::Link(key, "/programs/" + id ));//### in the ESP we are using 'String' so we need a different function
		}
		context.listOfNotAllowedKeys.push_back(key);//there wont be a key named program

		key = F("sprinkler");
		if(plant._sprinkler() != nullptr){//if sprinkler exists
			context.nextName = &key;
			const int& sid = plant._sprinkler()->id();
			char csid[5];
			itoa(sid, csid, 10);
			String id(csid);
			links.push_back(stf::Link(key, "/sprinklers/" + id));
		}
		context.listOfNotAllowedKeys.push_back(key);//we dont want a program key of null, no program = no key in the json file

		if(!links.empty())
			archive.addProperties(*this, MACRO_NVP(links));
	}

	template<typename ArchiveType>
	void Json2Model(garden::Plant& plant, ArchiveType& archive) {//we want the program and sprinkler keys to be the id instead of the whole object
		//context.listOfNotAllowedKeys.push_back("program");//kinda useless cause he wont find those keys anyways
		//context.listOfNotAllowedKeys.push_back("sprinkler");
		std::vector<stf::Link> links;
		archive.loadProperties(*this, MACRO_NVP(links));
		bool sawProgramLink = false;
		bool sawSprinklerLink = false;

		for(auto& link : links){
			if(link.rel == F("program") && !link.getHrefTokens().empty()){
				sawProgramLink = true;
				int pid = atoi(link.getHrefTokens().back().c_str());
				archive.addCallBack([pid ,&plant](){plant.setProgram(pid);});
			}
			else if(link.rel == F("sprinkler")) {
				sawSprinklerLink = true;
				int sid = atoi(link.getHrefTokens().back().c_str());
				archive.addCallBack([sid ,&plant](){plant.setSprinkler(sid);});
			}
		}

		if(!sawProgramLink){// if we didn't see it, it means we deleted it
			plant._program.set(nullptr);
		}
		if(!sawSprinklerLink){
			plant._sprinkler.set(nullptr);
		}

	}

	template<typename T, typename A>
	std::function<Model::Property<std::shared_ptr<T>>* (std::vector<Model::Property<std::shared_ptr<T>>, A>&, DAL::JsonContex&)> getComperatorForObjWithIdAsKey(std::vector<Model::Property<std::shared_ptr<T>>, A>& vec, DAL::JsonContex& contex){
		typedef Model::Property<std::shared_ptr<T>> type;
		auto lambdaComperatorFnc = [](std::vector<type, A>& vec, DAL::JsonContex& contex){
			int id = contex.getKeyValue("id").operator int();
			for(const type& t : vec){
				if(t.get()->id == id)
					return (type*) &t;
			}
			return (type*)NULL;
		};
		std::function<type* (std::vector<type, A>&, DAL::JsonContex&)> comperator = std::function<type* (std::vector<type, A>&, DAL::JsonContex&)>(lambdaComperatorFnc);

		removeIfNotInJsonProp(vec, contex);

		return comperator;
	}

	template<typename A>
	std::function<Model::Property<std::shared_ptr<garden::Sprinkler>>* (std::vector<Model::Property<std::shared_ptr<garden::Sprinkler>>, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<Model::Property<std::shared_ptr<garden::Sprinkler>>, A>& vec, DAL::JsonContex& contex)
	{
//		typedef Model::Property<std::shared_ptr<GardenModel::Sprinkler>> objType;
		//std::cout << "specialization getVectorComperator Model::Property<std::shared_ptr<GardenModel::Sprinkler>>" << std::endl;

		return getComperatorForObjWithIdAsKey(vec, contex);
	}

	template<typename A>
	std::function<Model::Property<std::shared_ptr<garden::SimpleProgram>>* (std::vector<Model::Property<std::shared_ptr<garden::SimpleProgram>>, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<Model::Property<std::shared_ptr<garden::SimpleProgram>>, A>& vec, DAL::JsonContex& contex)
	{
//		typedef Model::Property<std::shared_ptr<GardenModel::Sprinkler>> objType;
		//std::cout << "specialization getVectorComperator Model::Property<std::shared_ptr<GardenModel::SimpleProgram>>" << std::endl;

		return getComperatorForObjWithIdAsKey(vec, contex);
	}

	template<typename A>
	std::function<Model::Property<std::shared_ptr<garden::Plant>>* (std::vector<Model::Property<std::shared_ptr<garden::Plant>>, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<Model::Property<std::shared_ptr<garden::Plant>>, A>& vec, DAL::JsonContex& contex)
	{
//		typedef Model::Property<std::shared_ptr<GardenModel::Sprinkler>> objType;
		//std::cout << "specialization getVectorComperator Model::Property<std::shared_ptr<GardenModel::Plant>>" << std::endl;

		return getComperatorForObjWithIdAsKey(vec, contex);
	}

	template<typename T, typename A>//TODO: here i need to add enable_if T have id property also in the other one
	std::function<T* (std::vector<T, A>&, DAL::JsonContex&)> getComperatorForObjWithIdAsKeyNoPropOrSharedptr(std::vector<T, A>& vec, DAL::JsonContex& contex){
		auto lambdaComperatorFnc = [](std::vector<T, A>& vec, DAL::JsonContex& contex){
			int id = contex.getKeyValue("id").operator int();
			for(T& t : vec){
				if(t.id == id)
					return &t;
			}
			return (T*)NULL;
		};
		std::function<T* (std::vector<T, A>&, DAL::JsonContex&)> comperator = std::function<T* (std::vector<T, A>&, DAL::JsonContex&)>(lambdaComperatorFnc);

		removeIfNotInJson2(vec, contex);

		return comperator;
	}

	template<typename A> //TODO: you can do it with one template with enable_if and check if it have id
	std::function<garden::Hour* (std::vector<garden::Hour, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<garden::Hour, A>& vec, DAL::JsonContex& contex)
	{
//		typedef Model::Property<std::shared_ptr<GardenModel::Sprinkler>> objType;
		//std::cout << "specialization getVectorComperator Model::Property<std::shared_ptr<GardenModel::Plant>>" << std::endl;

		return getComperatorForObjWithIdAsKeyNoPropOrSharedptr(vec, contex);
	}

	template<typename A>
	std::function<garden::Day* (std::vector<garden::Day, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<garden::Day, A>& vec, DAL::JsonContex& contex)
	{
//		typedef Model::Property<std::shared_ptr<GardenModel::Sprinkler>> objType;
		//std::cout << "specialization getVectorComperator Model::Property<std::shared_ptr<GardenModel::Plant>>" << std::endl;

		return getComperatorForObjWithIdAsKeyNoPropOrSharedptr(vec, contex);
	}

	bool checkIfIdInJsonArray(int id, DAL::JsonContex& contex){
		if(contex.isJsonObject())
			Serial.println(F("ERROR: in checkIfIdInJsonArray"));

		JsonArray& jArr = *contex._jsonArr;
		for(int i = 0; i < jArr.size(); i++){
			JsonObject& innerContex = jArr[i];
			int jId = innerContex["id"];
			if(id == jId)
				return true;
		}
		return false;
	}

	template<typename T, typename A>
	bool removeIfNotInJsonProp(std::vector<T, A>& vec, DAL::JsonContex& contex){
		vec.erase(std::remove_if(vec.begin(), vec.end(), [&](T& prop) { return !checkIfIdInJsonArray(prop.get()->id, contex); }), vec.end());
	}

	template<typename T, typename A>
	void removeIfNotInJson2(std::vector<T, A>& vec, DAL::JsonContex& contex){
		vec.erase(std::remove_if(vec.begin(), vec.end(), [&](T& t) { return !checkIfIdInJsonArray(t.id, contex); }), vec.end());
	}

};

} /* namespace garden */

#endif /* GARDENMODULE_DAL_GARDENRESTMAPPINGFILE_H_ */
