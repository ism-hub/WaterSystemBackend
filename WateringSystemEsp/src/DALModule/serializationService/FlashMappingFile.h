/*
 * FlashMappingFile.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#ifndef DALMODULE_SERIALIZATIONSERVICE_FLASHMAPPINGFILE_H_
#define DALMODULE_SERIALIZATIONSERVICE_FLASHMAPPINGFILE_H_

#include <Garden.h>
#include <JsonContex.h>

namespace DAL {

class FlashMappingFile {//mapping file of what we are saving to the flash
public:

	template<typename ... Args>
	void Model2Json(Args& ...) {}

	template<typename ... Args>
	void Json2Model(Args& ...) {}

	template<typename ArchiveType>
	void Model2Json(const GardenModel::Plant& plant, ArchiveType& archive) {//we want the program and sprinkler keys to be the id instead of the whole object
		String key;
		JsonContex& context = archive.getCurrentContex();

		key = "program";
		if(plant._program() != nullptr){// if the plant have the program, adding a key with its id
			context.nextName = &key;
			const int& pid = plant._program()->id();
			context.add(pid);
		}else
			context.listOfNotAllowedKeys.push_back(key);//there wont be a key named program

		key = "sprinkler";
		if(plant._sprinkler() != nullptr){//if sprinkler exists
			context.nextName = &key;
			const int& sid = plant._sprinkler()->id();
			context.add(sid);
		}else
			context.listOfNotAllowedKeys.push_back(key);//we dont want a program key of null, no program = no key in the json file
	}

	template<typename ArchiveType>
	void Json2Model(GardenModel::Plant& plant, ArchiveType& archive) {//we want the program and sprinkler keys to be the id instead of the whole object
		JsonContex& context = archive.getCurrentContex();
		int programId=-1;
		archive.loadProperties(mycereal::make_nvp("program", programId));

		int sprinklerId=-1;
		archive.loadProperties(mycereal::make_nvp("sprinkler", sprinklerId));

		if(programId != -1)
			archive.addCallBack([programId ,&plant](){plant.setProgram(programId);});
		if(sprinklerId != -1)
			archive.addCallBack([sprinklerId ,&plant](){plant.setSprinkler(sprinklerId);});

		context.listOfNotAllowedKeys.push_back("program");//we already handled this so no need for the default handling to do it again
		context.listOfNotAllowedKeys.push_back("sprinkler");
	}

	template<typename Parent, typename ArchiveType>
	class PolyRet {
	public:


		template <typename T>
		operator T(){

		}
	};

	//every time we need to create an object (when loading) we will call this function
	//T - the type to be created
	//parent - the object which need the creation (if we can specify one) (pointer/vector/property )
	template<typename T, typename Parent, typename ArchiveType>
	PolyRet<Parent, ArchiveType> create(Parent* parent, ArchiveType archive){

	}
};

} /* namespace DAL */

#endif /* DALMODULE_SERIALIZATIONSERVICE_FLASHMAPPINGFILE_H_ */
