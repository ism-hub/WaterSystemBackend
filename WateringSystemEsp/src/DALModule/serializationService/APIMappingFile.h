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
#include <string>


namespace DAL {

class APIMappingFile {//mapping file of what we are saving to the flash
	class Link{
		std::vector<String> tokenizedHref;
	public:
		String rel;
		String href;

		Link(const String& rel, const String& href): rel(rel), href(href){}
		Link() {}

		const std::vector<String>& getHrefTokens(){//TODO: 111 After finishing the rest of the service: this code is suppose to do the same as the uri parser in the HttpServletRequest, extract that code to some lib
			if(tokenizedHref.size() > 0)//already tokenized
				return tokenizedHref;
			tokenizedHref = stru::split(href.c_str(), '/');
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

	template<typename ArchiveType>
	void Model2Json(const GardenModel::Plant& plant,  ArchiveType& archive) {//we want the program and sprinkler keys to be the id instead of the whole object
		String key;
		std::vector<Link> links;
		JsonContex& context = archive.getCurrentContex();

		key = "program";
		if(plant._program() != nullptr){// if the plant have the program, adding a key with its id
			context.nextName = &key;
			const int& pid = plant._program()->id();
			links.push_back(Link(key, "/programs/" + std::to_string(pid) ));//### in the ESP we are using 'String' so we need a different function
		}
		context.listOfNotAllowedKeys.push_back(key);//there wont be a key named program

		key = "sprinkler";
		if(plant._sprinkler() != nullptr){//if sprinkler exists
			context.nextName = &key;
			const int& sid = plant._sprinkler()->id();
			links.push_back(Link(key, "/sprinklers/" + std::to_string(sid)));
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

		for(auto& link : links){
			if(link.rel == "program" && !link.getHrefTokens().empty()){
				int pid = atoi(link.getHrefTokens().back().c_str());
				archive.addCallBack([pid ,&plant](){plant.setProgram(pid);});
			}
			else if(link.rel == "sprinkler") {
				int sid = atoi(link.getHrefTokens().back().c_str());
				archive.addCallBack([sid ,&plant](){plant.setSprinkler(sid);});
			}
		}
	}
};

} /* namespace DAL */

#endif /* DALMODULE_SERIALIZATIONSERVICE_APIMAPPINGFILE_H_ */
