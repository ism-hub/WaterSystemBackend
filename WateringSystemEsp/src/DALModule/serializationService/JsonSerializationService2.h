/*
 * SerializationService2.h
 *
 *  Created on: 8 áôáø× 2018
 *      Author: rina
 */

#ifndef SERIALIZATIONSERVICE2_H_
#define SERIALIZATIONSERVICE2_H_

//#include <iostream>
#include <vector>
#include <ArduinoJson.h>
#include <WString.h>
#include <ObserverDesignPattern/Property.hpp>
#include <algorithm>
#include <NameValuePair.h>
//#include <Sprinkler.h>

namespace DAL {

//-------------------------------------------- detail input archive


template <typename saveArchiveType, typename loadArchiveType>
class SerializationService2 {
	std::shared_ptr<saveArchiveType> _saveArchive;
	std::shared_ptr<loadArchiveType> _loadArchive;
public:

	SerializationService2(std::shared_ptr<saveArchiveType> saveArchive, std::shared_ptr<loadArchiveType> loadArchive) : _saveArchive(saveArchive), _loadArchive(loadArchive) {}
	virtual ~SerializationService2(){}

	template<typename ModelType, typename instream>
	void Model2Json(const ModelType& model, instream& stream){//const reference can take rvalues
		JsonObject& root = _saveArchive->Model2Json(model);
		String jsonStr;
		root.printTo(jsonStr);
		stream = jsonStr;
		//root.printTo(stream);
		_saveArchive->clear();
	}

	//loading only the keys that are in the json if no key is present we just ignore the property, even if it has nameValuePair
	template<typename ModelType>
	void Json2Model(ModelType& model, const String& json){//we are taking references and fill them
		_loadArchive->Json2Model(model, json);
		_loadArchive->clear();
	}


};

} /* namespace DAL */

#endif /* SERIALIZATIONSERVICE2_H_ */

// PUT THIS IN SOME OTHER MAIN FILE AND RUN (TO TEST)
/*

*/
