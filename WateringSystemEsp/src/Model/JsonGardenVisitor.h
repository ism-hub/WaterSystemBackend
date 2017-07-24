/*
 * JsonGardenVisitor.h
 *
 *  Created on: 22 αιεμι 2017
 *      Author: IsM
 */

#ifndef MODEL_JSONGARDENVISITOR_H_
#define MODEL_JSONGARDENVISITOR_H_

#include <GardenVisitor.h>
#include <wString.h>
#include <Garden.h>
#include <Plant.h>
#include <WString.h>
#include <stdio.h>


namespace GardenModel {

class JsonGardenVisitor: public GardenVisitor {//###you need to remember to free the pointers to the strings you allocate!!!
public:
	JsonGardenVisitor(){

	}
	virtual ~JsonGardenVisitor(){
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ JsonGardenVisitor DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	virtual void* visit(Garden& garden) {
		String& gardenJson = *(new String());
		gardenJson = "{\"plants\":[";
		String* strPlant;
		for (int i = 0; i < garden._plants.size(); ++i) {
			strPlant = (String*)(garden._plants[i]->accept(*this));
			gardenJson += *strPlant;
			gardenJson += ",";
			strPlant->~String();
		}
		gardenJson += "]}";
		return &gardenJson;
	}

	virtual void* visit(Plant& plant) {
		String& gardenJson = *(new String());
		gardenJson = "{\"sprinkler\":";
		String& strSprinkler = *(String*)plant._sprinkler->accept(*this);
		gardenJson += strSprinkler;
		gardenJson += "}";

		strSprinkler.~String();
		return &gardenJson;
	}

	virtual void* visit(Sprinkler& sprinkler) {
		String& gardenJson = *(new String("{"));

		char cId[3];
		snprintf(cId, sizeof(cId),"%d", sprinkler.getId());
		gardenJson += "\"id\":";
		gardenJson += cId;
		gardenJson += ",";

		gardenJson += "\"name\":";
		gardenJson += sprinkler.getName();
		gardenJson += ",";

		gardenJson += "\"status\":";
		if(sprinkler.getStatus() == Sprinkler::On)//### bad bad bad fix that
			gardenJson += "On";
		else
			gardenJson += "Off";
		gardenJson += "}";
		return &gardenJson;
	}


};

} /* namespace GardenModel */

#endif /* MODEL_JSONGARDENVISITOR_H_ */
