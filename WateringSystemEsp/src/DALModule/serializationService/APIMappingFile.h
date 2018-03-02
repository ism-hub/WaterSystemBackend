/*
 * APIMappingFile.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#ifndef DALMODULE_SERIALIZATIONSERVICE_APIMAPPINGFILE_H_
#define DALMODULE_SERIALIZATIONSERVICE_APIMAPPINGFILE_H_

#include <vector>
#include <algorithm>
#include <WString.h>
#include <StringUtils.h>
#include <NameValuePair.h>
#include <Garden.h>
#include <JsonContex.h>
#include <memory>
#include <Garden.h>
#include <ObserverDesignPattern/Property.hpp>

namespace DAL {

class APIMappingFile {//mapping file of what we are saving to the flash
	class Link{
		std::vector<String> tokenizedHref;

		std::vector<String> ssplit2(const char *str, char c = '/')
		{
			std::vector<String> result;
			String string(str);
			const char* start = str;
		    do
		    {
		        const char *begin = str;
		        while(*str != c && *str)
		            str++;
		        result.push_back(string.substring(begin - start, str - start));
		    } while (0 != *str++);

		    return result;
		}
		/*std::vector<String> ssplit2(const char *str, char c = '/')
		{
			std::vector<String> result;
		    do
		    {
		        const char *begin = str;
		        while(*str != c && *str)
		            str++;
		        result.push_back(String(begin, str));
		    } while (0 != *str++);

		    return result;
		}*/

	public:
		String rel;
		String href;

		Link(const String& rel, const String& href): rel(rel), href(href){}
		Link() {}

		const std::vector<String>& getHrefTokens(){//TODO: 111 After finishing the rest of the service: this code is suppose to do the same as the uri parser in the HttpServletRequest, extract that code to some lib
			if(tokenizedHref.size() > 0)//already tokenized
				return tokenizedHref;
			tokenizedHref = ssplit2(href.c_str(), '/');
			return tokenizedHref;
		}

		template <class Archive>
		void save(Archive& archive) const{
			archive.addProperties(MACRO_NVP(rel), MACRO_NVP(href));
		}

		template <class Archive>
		void load(Archive& archive) {
			archive.loadProperties(MACRO_NVP(rel), MACRO_NVP(href));
		}
	};
public:

	template<typename ... Args>
	void Model2Json(Args& ...) {}

	template<typename ... Args>
	void Json2Model(Args& ...) {}

	template<typename T, typename A>
	std::function<T* (std::vector<T, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<T, A>&, DAL::JsonContex&)
	{return std::function<T* (std::vector<T, A>&, DAL::JsonContex&)>();}

	template<typename ArchiveType>
	void Model2Json(const GardenModel::Plant& plant,  ArchiveType& archive) {//we want the program and sprinkler keys to be the id instead of the whole object
		String key;
		std::vector<Link> links;
		JsonContex& context = archive.getCurrentContex();

		key = "program";
		if(plant._program() != nullptr){// if the plant have the program, adding a key with its id
			context.nextName = &key;
			const int& pid = plant._program()->id();
			char cpid[5];
			itoa(pid, cpid, 10);// String(pid);
			String id(cpid);
			links.push_back(Link(key, "/programs/" + id ));//### in the ESP we are using 'String' so we need a different function
		}
		context.listOfNotAllowedKeys.push_back(key);//there wont be a key named program

		key = "sprinkler";
		if(plant._sprinkler() != nullptr){//if sprinkler exists
			context.nextName = &key;
			const int& sid = plant._sprinkler()->id();
			char csid[5];
			itoa(sid, csid, 10);
			String id(csid);
			links.push_back(Link(key, "/sprinklers/" + id));
		}
		context.listOfNotAllowedKeys.push_back(key);//we dont want a program key of null, no program = no key in the json file

		if(!links.empty())
			archive.addProperties(MACRO_NVP(links));
	}

	template<typename ArchiveType>
	void Json2Model(GardenModel::Plant& plant, ArchiveType& archive) {//we want the program and sprinkler keys to be the id instead of the whole object
		//context.listOfNotAllowedKeys.push_back("program");//kinda useless cause he wont find those keys anyways
		//context.listOfNotAllowedKeys.push_back("sprinkler");
		std::vector<Link> links;
		archive.loadProperties(MACRO_NVP(links));
		bool sawProgramLink = false;
		bool sawSprinklerLink = false;

		for(auto& link : links){
			if(link.rel == "program" && !link.getHrefTokens().empty()){
				sawProgramLink = true;
				int pid = atoi(link.getHrefTokens().back().c_str());
				archive.addCallBack([pid ,&plant](){plant.setProgram(pid);});
			}
			else if(link.rel == "sprinkler") {
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
	std::function<Model::Property<std::shared_ptr<GardenModel::Sprinkler>>* (std::vector<Model::Property<std::shared_ptr<GardenModel::Sprinkler>>, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<Model::Property<std::shared_ptr<GardenModel::Sprinkler>>, A>& vec, DAL::JsonContex& contex)
	{
//		typedef Model::Property<std::shared_ptr<GardenModel::Sprinkler>> objType;
		//std::cout << "specialization getVectorComperator Model::Property<std::shared_ptr<GardenModel::Sprinkler>>" << std::endl;

		return getComperatorForObjWithIdAsKey(vec, contex);
	}

	template<typename A>
	std::function<Model::Property<std::shared_ptr<GardenModel::SimpleProgram>>* (std::vector<Model::Property<std::shared_ptr<GardenModel::SimpleProgram>>, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<Model::Property<std::shared_ptr<GardenModel::SimpleProgram>>, A>& vec, DAL::JsonContex& contex)
	{
//		typedef Model::Property<std::shared_ptr<GardenModel::Sprinkler>> objType;
		//std::cout << "specialization getVectorComperator Model::Property<std::shared_ptr<GardenModel::SimpleProgram>>" << std::endl;

		return getComperatorForObjWithIdAsKey(vec, contex);
	}

	template<typename A>
	std::function<Model::Property<std::shared_ptr<GardenModel::Plant>>* (std::vector<Model::Property<std::shared_ptr<GardenModel::Plant>>, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<Model::Property<std::shared_ptr<GardenModel::Plant>>, A>& vec, DAL::JsonContex& contex)
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
	std::function<GardenModel::Hour* (std::vector<GardenModel::Hour, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<GardenModel::Hour, A>& vec, DAL::JsonContex& contex)
	{
//		typedef Model::Property<std::shared_ptr<GardenModel::Sprinkler>> objType;
		//std::cout << "specialization getVectorComperator Model::Property<std::shared_ptr<GardenModel::Plant>>" << std::endl;

		return getComperatorForObjWithIdAsKeyNoPropOrSharedptr(vec, contex);
	}

	template<typename A>
	std::function<GardenModel::Day* (std::vector<GardenModel::Day, A>&, DAL::JsonContex&)> getVectorComperator(std::vector<GardenModel::Day, A>& vec, DAL::JsonContex& contex)
	{
//		typedef Model::Property<std::shared_ptr<GardenModel::Sprinkler>> objType;
		//std::cout << "specialization getVectorComperator Model::Property<std::shared_ptr<GardenModel::Plant>>" << std::endl;

		return getComperatorForObjWithIdAsKeyNoPropOrSharedptr(vec, contex);
	}

	bool checkIfIdInJsonArray(int id, DAL::JsonContex& contex){
		if(contex.isJsonObject())
			Serial.println("ERROR: in checkIfIdInJsonArray");

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

} /* namespace DAL */

#endif /* DALMODULE_SERIALIZATIONSERVICE_APIMAPPINGFILE_H_ */
