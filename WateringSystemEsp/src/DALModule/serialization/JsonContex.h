/*
 * JsonContex.h
 *
 *  Created on: 16 ����� 2018
 *      Author: IsM
 */

#ifndef DALMODULE_SERIALIZATION_JSONCONTEX_H_
#define DALMODULE_SERIALIZATION_JSONCONTEX_H_

#include <ArduinoJson.h>
#include <vector>

namespace DAL {

class JsonContex {
	bool _isJsonObj;
public:
	JsonObject* _jsonObj;
	JsonArray* _jsonArr;
	const String* nextName ;//next Key to read/write to
	std::vector<String> listOfNotAllowedKeys;// list of names that we not aloowed as keys in this context,
	int nextIndx=0;// like nextName only for arrays
	JsonContex(JsonObject* jsonObj): _isJsonObj(true), _jsonObj(jsonObj), _jsonArr(NULL), nextName(NULL) {}
	JsonContex(JsonArray* jsonArr): _isJsonObj(false), _jsonObj(NULL), _jsonArr(jsonArr), nextName(NULL) {}
	//JsonContex(JsonContex&& other) = default;
	//JsonContex(JsonContex& other) = default;

	bool isJsonObject(){
		return _isJsonObj;
	}

	JsonContex createNestedObject() {
		if(_isJsonObj)
			return JsonContex(&(_jsonObj->createNestedObject(*nextName)));
		return JsonContex(&_jsonArr->createNestedObject());
	}

	JsonContex createNestedArray() {
		if(_isJsonObj)
			return &_jsonObj->createNestedArray(*nextName);
		return &_jsonArr->createNestedArray();
	}

	bool isNextKeyIsNull(){
		auto chp = getKeyValue();
		if(_isJsonObj){
			auto multiVal = (*(_jsonObj))[*(nextName)];
			return (multiVal.is<char*>() && multiVal.as<char*>() == nullptr);
		}
		else{
			auto multiVal = (*(_jsonArr))[nextIndx];
			return (multiVal.is<char*>() && multiVal.as<char*>() == nullptr);
		}

	}

	//revisit the reference stuff
	template<typename T>
	bool add(const T& t){
		if(_isJsonObj)
			return _jsonObj->set(*nextName, t);
		return _jsonArr->add(t);
	}

	template<typename TString>
	bool containsKey(const TString& key){
		if(!_isJsonObj)
			return false;
		return _jsonObj->containsKey(key);
	}

	class opTgetKet{
	public:
		JsonContex& jCntx;
		opTgetKet(JsonContex& jsonCntx) : jCntx(jsonCntx){}

		template<typename T>
		operator T(){
			if(jCntx._isJsonObj)
				return (*(jCntx._jsonObj))[*(jCntx.nextName)];
			else
				return (*(jCntx._jsonArr))[jCntx.nextIndx];
		}
	};

	opTgetKet getKeyValue() {//TODO: implement properly
		return opTgetKet(*this);
	}

};

} /* namespace DAL */

#endif /* DALMODULE_SERIALIZATION_JSONCONTEX_H_ */