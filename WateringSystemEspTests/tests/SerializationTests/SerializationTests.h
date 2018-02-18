/*
 * SerializationTests.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#ifndef SERIALIZATIONTESTS_SERIALIZATIONTESTS_H_
#define SERIALIZATIONTESTS_SERIALIZATIONTESTS_H_

#include <memory>
#include <iostream>
#include <stdio.h>
#include <JsonSerializationService2.h>
#include <Sprinkler.h>
#include <Plant.h>
#include <Garden.h>
#include <TimePattern.h>
#include <SimpleProgram.h>
#include <WString.h>
#include <StringUtils.h>
#include <DoNothingMappingFile.h>
#include <JsonLoadArchive.h>
#include <JsonSaveArchive.h>
#include <assert.h>
#include <FlashMappingFile.h>
#include <APIMappingFile.h>

namespace SerializationTests {

using namespace std;

const int numberOfDifferentGardens = 4;

GardenModel::Garden createGarden(int gardenIndx){
	shared_ptr<GardenModel::Sprinkler> sprinkler = make_shared<GardenModel::Sprinkler>();
	sprinkler->id = gardenIndx*3;
	vector<GardenModel::Hour> hours = {GardenModel::Hour(gardenIndx*2, gardenIndx*3)};
	hours[0].id = gardenIndx*3;
	std::vector<GardenModel::Day> days = {GardenModel::Day(hours)};
	days[0].id = gardenIndx*3;
	GardenModel::TimePattern TPattern(days);
	std::shared_ptr<GardenModel::SimpleProgram> simpleProgram = std::make_shared<GardenModel::SimpleProgram>();
	simpleProgram->timePattern = TPattern;
	std::shared_ptr<GardenModel::Plant> plant;

	switch(gardenIndx) {
	    case 1: plant = std::make_shared<GardenModel::Plant>(nullptr, nullptr);
	            break;
	    case 2: plant = std::make_shared<GardenModel::Plant>(nullptr, simpleProgram);
	    	    break;
	    case 3: plant = std::make_shared<GardenModel::Plant>(sprinkler, nullptr);
	    	    break;
	    case 4: plant = std::make_shared<GardenModel::Plant>(sprinkler, simpleProgram);
	    	    break;
	    default: plant = std::make_shared<GardenModel::Plant>(sprinkler, simpleProgram);
	             break;
	}

	plant->id = gardenIndx*6;

	GardenModel::Garden garden;
	garden._plants.push_back(plant);
	garden._sprinklers.push_back(sprinkler);
	garden._programs.push_back(simpleProgram);

	return garden;
}

//memory leaks lane
template<typename MappingFileType>
DAL::SerializationService2<mycereal::JsonSaveArchive<MappingFileType>, mycereal::JsonLoadArchive<MappingFileType> >& createSerializationServer(MappingFileType& mappingFile) {
	typedef mycereal::JsonLoadArchive<MappingFileType> LoadArchiveType;
	typedef mycereal::JsonSaveArchive<MappingFileType> SaveArchiveType;
	auto& loadArchive = *(new LoadArchiveType(mappingFile));
	auto& saveArchive = *(new SaveArchiveType(mappingFile));
	auto& serSevice = *(new DAL::SerializationService2<SaveArchiveType, LoadArchiveType >(saveArchive, loadArchive));
	return serSevice;
}

//checking if serializing -> deserializing -> serializing gives the same json string both times
template<typename SerializationServiceType>
bool serDeserSerTest(GardenModel::Garden& garden, SerializationServiceType& serService){

	String firsSer;
	serService.Model2Json(garden, firsSer);

	//cout << firsSer << endl;

	GardenModel::Garden emptyGarden;
	serService.Json2Model(emptyGarden, firsSer);//"{\"name\":\"GardName12\",\"plants\":[{\"id\":20,\"name\":\"Li2ly\",\"program\":{\"id\":20,\"name\":\"not2-set\",\"timePattern\":{\"days\":[{\"id\":20,\"hours\":[{\"id\":20,\"hour\":124,\"min\":122}]}]}}}],\"sprinklers\":[{\"id\":12,\"status\":\"On\"}],\"programs\":[{\"id\":20,\"name\":\"not2-set\",\"timePattern\":{\"days\":[{\"id\":20,\"hours\":[{\"id\":20,\"hour\":124,\"min\":122}]}]}}]}");
	String secSer;
	serService.Model2Json(emptyGarden, secSer);

	//cout << secSer << endl;

	return(firsSer == secSer);
}

/*template<typename MappingFileType>
void TestMappingFile(MappingFileType& mappingFile, int gardenVariety){

	GardenModel::Garden garden = createGarden(gardenVariety);
	auto& serServive = createSerializationServer<MappingFileType>(mappingFile);

	serDeserSerTest(garden, serServive);

}*/

//testing the serialization (without mapping file (using the default one))
void DoNothingMappingFileTest(int gardenVariety = 4){

	GardenModel::Garden garden = createGarden(gardenVariety);
	mycereal::DoNothingMappingFile doNothingMappingFile;
	auto& serServive = createSerializationServer<mycereal::DoNothingMappingFile>(doNothingMappingFile);

	assert(serDeserSerTest(garden, serServive));
}

//testing the FlashMapping
void FlashMappingFileTest(int gardenVariety = 4){

	GardenModel::Garden garden = createGarden(gardenVariety);
	DAL::FlashMappingFile flashMappingFile;
	auto& serServive = createSerializationServer<DAL::FlashMappingFile>(flashMappingFile);

	assert(serDeserSerTest(garden, serServive));
}

void APIMappingFileTest(int gardenVariety = 4){

	GardenModel::Garden garden = createGarden(gardenVariety);
	DAL::APIMappingFile apiMappingFile;
	auto& serServive = createSerializationServer<DAL::APIMappingFile>(apiMappingFile);

	assert(serDeserSerTest(garden, serServive));
}

void run(){
	for(int i = 1; i <= numberOfDifferentGardens; i++){
		DoNothingMappingFileTest(i);
		FlashMappingFileTest(i);
		APIMappingFileTest(i);
	}


}



} /* namespace SerializationTests */

#endif /* SERIALIZATIONTESTS_SERIALIZATIONTESTS_H_ */
