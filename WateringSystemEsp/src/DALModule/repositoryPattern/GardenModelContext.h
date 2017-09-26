/*
 * DBContext.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_GARDENMODELCONTEXT_H_
#define DAL_GARDENMODELCONTEXT_H_

#include <DALModule/serialization/cereal2.h>
#include <DALModule/serialization/json2.h>
#include <Garden.h>

#include <FS.h>

#include <DALModule/serializationService/JsonSerializationService.h>

//#include <JsonGardenVisitor.h>
//#include <GardenRepository.h>
//#include <PlantRepository.h>
//#include <SprinklerRepository.h>

//class GardenRepository;
//class PlantRepository;
//class SprinklerRepository;

using namespace GardenModel;

namespace DAL {

class GardenModelContext {
public:
	shared_ptr<Garden> 				_garden = nullptr;
	std::shared_ptr<DAL::JsonSerializationService> _jsonSerializationService;

	GardenModelContext(std::shared_ptr<DAL::JsonSerializationService> jsonSerializationService) :
		_jsonSerializationService(jsonSerializationService)
	{

	}

	virtual ~GardenModelContext(){

	}

	shared_ptr<Garden> getGarden(){//init if needed
		if(_garden == nullptr)
			init();
		return _garden;
	}


	bool init(){//loads the data from flash to _garden, (if you already loaded, disposed the old copy and load a fresh one)
		return loadGardenIntoMemory();
	}

	bool saveChanges(){//writes the data into flash
		if(_garden == nullptr)
			return false;

		//convert the object into json string
	//	rapidjson::StringBuffer jsonSBuffer;
	//	rapidjson::Writer<rapidjson::StringBuffer> writer(jsonSBuffer);

	//	cereal2::JSONOutputArchive& archive = *(new cereal2::JSONOutputArchive(writer));
	//	archive(CEREAL2_NVP(_garden));
	//	archive.~JSONOutputArchive();

		//save the string into flash
		bool result = SPIFFS.begin();
		Serial.println("SPIFFS opened: " + result);

		File f = SPIFFS.open("/f.txt", "w");
		//f.print(jsonSBuffer.GetString());
		f.print(_jsonSerializationService->modelToJson(*_garden));

		f.close();

		Serial.println("SPIFFS closing: " );
		SPIFFS.end();



		return true;
	}

private:
	bool loadGardenIntoMemory() {//from flash to object

		//load our json string from the flash
		String jsonGarden;

		  bool result = SPIFFS.begin();
		  Serial.println("SPIFFS opened: " + result);


		  File f = SPIFFS.open("/f.txt", "r");

		  if (!f) {//File doesn't exist yet.
			  _garden = nullptr;
			  return false;//_garden is null
		  }
		  else{
			  jsonGarden = f.readStringUntil('\0');
			 // _jsonSerializationService->_stringBuffer.Put(f.readStringUntil('\0')) ;
			  f.close();
		  }

		  Serial.println("SPIFFS closing: " );
		  SPIFFS.end();


		//convert the json into garden
	//	rapidjson::StringStream jsonInput(jsonGarden.c_str());
	//	cereal2::JSONInputArchive readArchive(jsonInput); //(istreamChar);

		if(_garden != nullptr)
			_garden = nullptr;
	//	_garden = make_shared<Garden>();

	//	readArchive(_garden);

		  _garden = _jsonSerializationService->jsonToModel<GardenModel::Garden>(jsonGarden);//(jsonGarden.c_str());

		return true;
	}
};

} /* namespace Http */

#endif /* DAL_GARDENMODELCONTEXT_H_ */
