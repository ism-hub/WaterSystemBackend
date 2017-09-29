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
	std::shared_ptr<Garden> 				_garden = nullptr;
	std::shared_ptr<DAL::JsonSerializationService> _jsonSerializationService;

	GardenModelContext(std::shared_ptr<DAL::JsonSerializationService> jsonSerializationService) :
		_jsonSerializationService(jsonSerializationService)
	{

	}

	virtual ~GardenModelContext(){

	}

	std::shared_ptr<Garden> getGarden(){//init if needed
		if(_garden == nullptr)
			init();
		return _garden;
	}


	bool init(){//loads the data from flash to _garden, (if you already loaded, disposed the old copy and load a fresh one)
		Serial.println("GardenModelContext init();");
		return loadGardenIntoMemory();
	}

	bool saveChanges() { //writes the data into flash
		if (_garden == nullptr)
			return false;

		String jsonStringOfOurGarden = _jsonSerializationService->modelToJson(*_garden);
		Serial.print("The json strin of the garden we saving to memory: ");
		Serial.println(jsonStringOfOurGarden);

		//save the string into flash
		bool result = SPIFFS.begin();
		Serial.println("SPIFFS opened: " + result);

		{
			File f = SPIFFS.open("/f.txt", "w");
			//f.print(jsonSBuffer.GetString());
			f.print(jsonStringOfOurGarden);
			f.flush();
			f.close();
		}
		Serial.println("SPIFFS closing: ");
		SPIFFS.end();

		return true;
	}

private:
	bool loadGardenIntoMemory() { //from flash to object

		//load our json string from the flash
		String jsonGarden;

		{
			bool result = SPIFFS.begin();
			Serial.println("SPIFFS opened: " + result);

			File f = SPIFFS.open("/f.txt", "r");

			if (!f) { //File doesn't exist yet.
				_garden = std::make_shared<Garden>(); //making empty garden
				f.close();
				SPIFFS.end();
				return false; //_garden is null
			} else {
				jsonGarden = f.readStringUntil('\0');
				f.flush();
				f.close();
			}

			Serial.println("SPIFFS closing");
			SPIFFS.end();
		}

		if (_garden != nullptr)
			_garden = nullptr;

		Serial.println("Reading the garden from the flash (as json) and realizing it ");
		_garden = _jsonSerializationService->jsonToModel<GardenModel::Garden>(jsonGarden);

		return true;
	}
};

} /* namespace Http */

#endif /* DAL_GARDENMODELCONTEXT_H_ */
