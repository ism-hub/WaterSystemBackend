/*
 * IModelContex.h
 *
 *  Created on: 27 αιεμι 2018
 *      Author: IsM
 */

#ifndef DALFRAMEWORK_SPIFFMODELCONTEX_H_
#define DALFRAMEWORK_SPIFFMODELCONTEX_H_

#include <DALFramework/IModelContex.h>
#include <memory>
#include <WString.h>

#include <FS.h>


namespace dalfw {

template <class SerializationSericeType, class ModelType>
class SPIFFModelContex : public IModelContex<ModelType> {
	std::shared_ptr<ModelType> 					_model = nullptr;
	std::shared_ptr<SerializationSericeType> 	_serializationService;
	String savePath;
public:
	SPIFFModelContex(std::shared_ptr<SerializationSericeType> serializationService, String savePath) :
		_serializationService(serializationService), savePath(savePath)
	{}
	virtual ~SPIFFModelContex() {}


	virtual std::shared_ptr<ModelType> get() override {
			if(_model == nullptr)
				init();
			return _model;
		}

	bool init() override {//loads the data from flash to _garden, (if you already loaded, disposed the old copy and load a fresh one)
		return loadIntoMemory();
	}

	bool save() override { //writes the data into flash
		if (_model == nullptr)
			return false;

		String jsonStringOfModel;
		_serializationService->Model2Json(*_model, jsonStringOfModel);

		//save the string into flash
		bool result = SPIFFS.begin();
		{
			File f = SPIFFS.open(savePath, "w");
			f.print(jsonStringOfModel);
			f.flush();
			f.close();
		}
		SPIFFS.end();

		return true;
	}

private:
	bool loadIntoMemory() { //from flash to object

	//load our json string from the flash

	String strModel;

	{
		bool result = SPIFFS.begin();
		if(!result){
			Serial.println("____EROR____ in loadIntoMemory SPIFFS not opened");
			return false;
		}

		File f = SPIFFS.open(savePath, "r");

		if (!f) { //File doesn't exist yet.
			Serial.println("file doesn't exists");
			_model = std::make_shared<ModelType>(); //making empty model
			f.close();
			SPIFFS.end();
			return false; //_garden is null
		} else {
			strModel = f.readStringUntil('\0');
			f.flush();
			f.close();
		}

		SPIFFS.end();
	}

	_model = std::make_shared<ModelType>();
	_serializationService->template Json2Model<ModelType>(*_model, strModel);
	return true;
}

};

} /* namespace dalfw */

#endif /* DALFRAMEWORK_SPIFFMODELCONTEX_H_ */
