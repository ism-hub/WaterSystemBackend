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
const int uniqueId = 12345;

garden::Garden createGarden(int gardenIndx){
	shared_ptr<garden::Sprinkler> sprinkler = make_shared<garden::Sprinkler>();
	sprinkler->id = gardenIndx*3;
	vector<garden::Hour> hours = {garden::Hour(gardenIndx*2, gardenIndx*3)};
	hours[0].id = gardenIndx*3;
	std::vector<garden::Day> days = {garden::Day(hours)};
	days[0].id = gardenIndx*3;
	garden::TimePattern TPattern(days);
	std::shared_ptr<garden::SimpleProgram> simpleProgram = std::make_shared<garden::SimpleProgram>();
	simpleProgram->timePattern = TPattern;
	std::shared_ptr<garden::Plant> plant;

	switch(gardenIndx) {
	    case 1: plant = std::make_shared<garden::Plant>(nullptr, nullptr);
	            break;
	    case 2: plant = std::make_shared<garden::Plant>(nullptr, simpleProgram);
	    	    break;
	    case 3: plant = std::make_shared<garden::Plant>(sprinkler, nullptr);
	    	    break;
	    case 4: plant = std::make_shared<garden::Plant>(sprinkler, simpleProgram);
	    	    break;
	    default: plant = std::make_shared<garden::Plant>(sprinkler, simpleProgram);
	             break;
	}

	plant->id = gardenIndx*6;

	garden::Garden garden;
	garden._plants.push_back(plant);
	garden._sprinklers.push_back(sprinkler);
	garden._programs.push_back(simpleProgram);

	return garden;
}

//memory leaks lane
template<typename MappingFileType>
std::shared_ptr<DAL::SerializationService2<mycereal::JsonSaveArchive<MappingFileType>, mycereal::JsonLoadArchive<MappingFileType>> > createSerializationServer(MappingFileType& mappingFile) {
	typedef mycereal::JsonLoadArchive<MappingFileType> LoadArchiveType;
	typedef mycereal::JsonSaveArchive<MappingFileType> SaveArchiveType;
	auto loadArchive = std::make_shared<LoadArchiveType>(mappingFile);
	auto saveArchive = std::make_shared<SaveArchiveType>(mappingFile);
	auto serSevice = std::make_shared<DAL::SerializationService2<SaveArchiveType, LoadArchiveType >>(saveArchive, loadArchive);
	return serSevice;
}

//checking if serializing -> deserializing -> serializing gives the same json string both times
template<typename SerializationServiceType>
bool serDeserSerTest(garden::Garden& garden, SerializationServiceType& serService){

	String firsSer;
	serService.Model2Json(garden, firsSer);

	//cout << firsSer << endl;

	garden::Garden emptyGarden;
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

	garden::Garden garden = createGarden(gardenVariety);
	mycereal::DoNothingMappingFile doNothingMappingFile;
	auto serServive = createSerializationServer<mycereal::DoNothingMappingFile>(doNothingMappingFile);

	assert(serDeserSerTest(garden, *serServive));
}

//testing the FlashMapping
void FlashMappingFileTest(int gardenVariety = 4){

	garden::Garden garden = createGarden(gardenVariety);
	DAL::FlashMappingFile flashMappingFile;
	auto serServive = createSerializationServer<DAL::FlashMappingFile>(flashMappingFile);

	assert(serDeserSerTest(garden, *serServive));
}

void APIMappingFileTest(int gardenVariety = 4){

	garden::Garden garden = createGarden(gardenVariety);
	DAL::APIMappingFile apiMappingFile;
	auto serServive = createSerializationServer<DAL::APIMappingFile>(apiMappingFile);

	assert(serDeserSerTest(garden, *serServive));
}

//we do it buy creating a model -> serializing it -> deserialize the created json on the same model -> serializing that model and asserting the json is the same as the first one
void APIMappingFileMergingJsonArrayIntoExistingVectorTest(int gardenVariety = 4)
{
	garden::Garden garden = createGarden(gardenVariety);
	DAL::APIMappingFile apiMappingFile;
	auto serServivePtr = createSerializationServer<DAL::APIMappingFile>(apiMappingFile);
	auto& serServive = *serServivePtr;

	String firstJson;
	serServive.Model2Json(garden, firstJson);

	serServive.Json2Model(garden, firstJson);

	String secondJson;
	serServive.Model2Json(garden, secondJson);

	//cout << "Before: " << firstJson << endl;
	//cout << "After: " << secondJson << endl;
	assert(firstJson == secondJson);
}

void APIMappingFileMergingJsonArrayIntoExistingVectorDeletesFromVecIfNotInJsonTest(int gardenVariety = 4){
	garden::Garden garden = createGarden(gardenVariety);
	DAL::APIMappingFile apiMappingFile;
	auto serServivePtr = createSerializationServer<DAL::APIMappingFile>(apiMappingFile);
	auto serServive = *serServivePtr;

	String json;
	serServive.Model2Json(garden, json);

	std::shared_ptr<garden::Sprinkler> sprinkler = make_shared<garden::Sprinkler>();
	sprinkler->id = uniqueId;
	garden._sprinklers.push_back(sprinkler);

	std::shared_ptr<garden::Plant> plant = make_shared<garden::Plant>();
	plant->id = uniqueId;
	garden._plants.push_back(plant);

	std::shared_ptr<garden::SimpleProgram> program = make_shared<garden::SimpleProgram>();;
	program->id = uniqueId;
	garden._programs.push_back(program);

	String jsonAfterAddingNewStuff;
	serServive.Model2Json(garden, jsonAfterAddingNewStuff);

	serServive.Json2Model(garden, json);//suppose to delete all that we add (not in json = not in model)

	String jsonAfterMerging;
	serServive.Model2Json(garden, jsonAfterMerging);

	//cout << "Before: " << json << endl;
	//cout << "Middle: " << jsonAfterAddingNewStuff << endl;
	//cout << "After: " << jsonAfterMerging << endl;

	assert(json == jsonAfterMerging);
}

void APIMappingFileDeletingLinks(int gardenVariety = 4){
	garden::Garden garden = createGarden(gardenVariety);
	DAL::APIMappingFile apiMappingFile;
	auto serService = createSerializationServer<DAL::APIMappingFile>(apiMappingFile);

	String json;
	serService->Model2Json(garden, json);

	bool alwaysPass = true;

	for(const auto& plant : garden._plants.getInnerVector()){
		if(plant.get()->_program != nullptr || plant.get()->_sprinkler != nullptr)
			alwaysPass = false;
		plant.get()->_program = nullptr;
		plant.get()->_sprinkler = nullptr;
	}

	String jsonWithoutPlantProgSprink;
	serService->Model2Json(garden, jsonWithoutPlantProgSprink);

	assert(alwaysPass || (jsonWithoutPlantProgSprink != json));

}

void temp(int gardenVariety = 4){
	garden::Garden garden; //= createGarden(gardenVariety);
	DAL::APIMappingFile apiMappingFile;
	auto serService = createSerializationServer<DAL::APIMappingFile>(apiMappingFile);

	const String json1 =
			"{\"name\":\"Food Garden\",\"plants\":[{\"links\":[{\"rel\":\"program\",\"href\":\"/programs/123\"},{\"rel\":\"sprinkler\",\"href\":\"/sprinklers/1\"}],\"id\":11,\"name\":\"plantA22\"}],\"sprinklers\":[{\"id\":1,\"status\":\"On\"}],\"programs\":[{\"id\":123,\"name\":\"simpleProgramA\",\"timePattern\":{\"days\":[{\"id\":80,\"hours\":[{\"id\":111,\"hour\":9,\"min\":30}]}]}}]}";
	serService->Json2Model(garden, json1);

	String json2;
	serService->Model2Json(garden, json2);
	cout << json2 << endl;

	String json3 =
			"{\"name\":\"Food Garden\",\"plants\":[{\"links\":[{\"rel\":\"sprinkler\",\"href\":\"/sprinklers/1\"}],\"id\":11,\"name\":\"plantA22\"}],\"programs\":[{\"id\":123,\"name\":\"simpleProgramA\",\"timePattern\":{\"days\":[{\"id\":80,\"hours\":[{\"id\":111,\"hour\":9,\"min\":30}]}]}}],\"sprinklers\":[{\"id\":1,\"status\":\"On\"}]}";
	serService->Json2Model(garden, json3);

	String json4;
	serService->Model2Json(garden, json4);
	cout << json4 << endl;


}

void run(){
	temp();
	/*for(int i = 1; i <= numberOfDifferentGardens; i++){
		DoNothingMappingFileTest(i);
		FlashMappingFileTest(i);
		APIMappingFileTest(i);
		APIMappingFileMergingJsonArrayIntoExistingVectorTest(i);
		APIMappingFileMergingJsonArrayIntoExistingVectorDeletesFromVecIfNotInJsonTest(i);
		APIMappingFileDeletingLinks(i);
	}*/
}



} /* namespace SerializationTests */

#endif /* SERIALIZATIONTESTS_SERIALIZATIONTESTS_H_ */
