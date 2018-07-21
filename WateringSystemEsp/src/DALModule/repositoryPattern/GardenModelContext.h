/*
 * DBContext.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_GARDENMODELCONTEXT_H_
#define DAL_GARDENMODELCONTEXT_H_

//#include <DALModule/serialization/cereal2.h>
//#include <DALModule/serialization/json2.h>
#include <Garden.h>

#include <FS.h>

#include <JsonSerializationService2.h>
#include <JsonSaveArchive.h>
#include <JsonLoadArchive.h>
#include <FlashMappingFile.h>
#include <IGardenModelContex.h>

//#include <JsonGardenVisitor.h>
//#include <GardenRepository.h>
//#include <PlantRepository.h>
//#include <SprinklerRepository.h>

//class GardenRepository;
//class PlantRepository;
//class SprinklerRepository;

using namespace GardenModel;

namespace DAL {
typedef DAL::SerializationService2< mycereal::JsonSaveArchive<DAL::FlashMappingFile>,mycereal::JsonLoadArchive<DAL::FlashMappingFile>> FlashSerializationSerice;
class GardenModelContext : public IGardenModelContex {
public:
	std::shared_ptr<Garden> 				_garden = nullptr;
	std::shared_ptr<FlashSerializationSerice> _jsonSerializationService;

	GardenModelContext(std::shared_ptr<FlashSerializationSerice> jsonSerializationService) :
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
		return loadGardenIntoMemory();
	}

	bool saveChanges() { //writes the data into flash
		if (_garden == nullptr)
			return false;

		String jsonStringOfOurGarden;
		_jsonSerializationService->Model2Json(*_garden, jsonStringOfOurGarden);

#ifdef DEBUG_MY_CODE
		Serial.print("The json strin of the garden we saving to memory: ");
		Serial.println(jsonStringOfOurGarden);
#endif
		//save the string into flash
		bool result = SPIFFS.begin();
		{
			File f = SPIFFS.open("/f.txt", "w");
			//f.print(jsonSBuffer.GetString());
			f.print(jsonStringOfOurGarden);
			f.flush();
			f.close();
		}
		SPIFFS.end();

		return true;
	}

private:
	bool loadGardenIntoMemory() { //from flash to object

		//load our json string from the flash
#ifdef DEBUG_MY_CODE
		Serial.println("Loading the garden from Flash to memory");
#endif

		String jsonGarden;

		{
			bool result = SPIFFS.begin();
			if(!result){
				Serial.println("____EROR____ in loadGardenIntoMemory SPIFFS not opened");
				return false;
			}


			File f = SPIFFS.open("/f.txt", "r");

			if (!f) { //File doesn't exist yet.
				Serial.println("file doesn't exists");
				_garden = std::make_shared<Garden>(); //making empty garden
				f.close();
				SPIFFS.end();
				return false; //_garden is null
			} else {
				jsonGarden = f.readStringUntil('\0');
				f.flush();
				f.close();
			}

			SPIFFS.end();
		}

		/*Serial.println("wtf?!?!?!");
		if (_garden != nullptr){
			Serial.println("wtf?!?!?!222");
			_garden = nullptr;
			Serial.println("kms");
		}*/

		_garden = std::make_shared<Garden>();
		_jsonSerializationService->Json2Model<GardenModel::Garden>(*_garden, jsonGarden);
		return true;
	}
};

} /* namespace Http */

#endif /* DAL_GARDENMODELCONTEXT_H_ */
