/*
 * HardwareTests.h
 *
 *  Created on: 12 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWARETESTS_HARDWARETESTS_H_
#define HARDWARETESTS_HARDWARETESTS_H_

#include <PlantsSuiteController.h>
#include <SprinklersController.h>
#include <PumpController.h>
#include <GardenUnitOfWork.h>
#include <IGardenModelContex.h>
#include <ChipSelect.h>
#include <chipSelectSPIService.h>

#include <algorithm>

//testing that the plantSuite execute the program on the plant
namespace HardwareTests {

class TestGardenModelContext : public DAL::IGardenModelContex {
public:
	std::shared_ptr<garden::Garden>  garden;
	TestGardenModelContext() {

	}
	virtual ~TestGardenModelContext() {}

	std::shared_ptr<garden::Garden> get() {
		if(garden == nullptr)
			init();
		return garden;
	}

	bool init() {
		garden = std::make_shared<garden::Garden>();
		return true;
	}

	bool save() {
		return true;
	}

};

template<class T>
void printTime(T timeToday){
	Serial.print("The time is:");
	Serial.print(timeToday.hours().count());
	Serial.print(":");
	Serial.print(timeToday.minutes().count());
	Serial.print(":");
	Serial.print((long)timeToday.seconds().count());
	Serial.println("");
}


void run(){
	// we will make a unit of work (garden) create the controllers for it
	// for now we change and check visually that the change is happening
	// after, we change our code to be a little more testable (base controller which we can override the events handlers)


	//################ hardware stuff ################
	//creating the SPI service and adding our chips to it
	std::shared_ptr<hrdwrctrl::SPIService> spiService = std::make_shared<hrdwrctrl::SPIService>();

	//	create & add the chip select
	hrdwrctrl::chip chipSelectSpiConf{"ChipSelect", 2};
	hrdwrctrl::ChipSelect::ChipSelectConfig chipSelectConf = hrdwrctrl::ChipSelect::ChipSelectConfig(8, chipSelectSpiConf);
	auto chipSelect = std::make_shared<hrdwrctrl::ChipSelect>(spiService, chipSelectConf);

	//	create the chipSelectSPIService
	std::shared_ptr<hrdwrctrl::chipSelectSPIService> chipSelectSPIService = std::make_shared<hrdwrctrl::chipSelectSPIService>(chipSelect);

	//	create & add the switchArray chip
	hrdwrctrl::chip spiConfForSwitchArray;
	spiConfForSwitchArray.name = "switch hub";
	spiConfForSwitchArray.selectPin = 1;
	hrdwrctrl::SwitchArray::SwitchArrayConfig config{spiConfForSwitchArray, 8};
	std::shared_ptr<hrdwrctrl::SwitchArray> switchHub = std::make_shared<hrdwrctrl::SwitchArray>(chipSelectSPIService, config);//NOTICE - we gave it the chipSelectSPIService cause it on the chipSelect chip
	//################ hardware stuff ################


	//################ DAL stuff ################
	//creating the Model stuff
	std::shared_ptr<TestGardenModelContext> gardenContext = std::make_shared<TestGardenModelContext>();
	std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork = std::make_shared<DAL::GardenUnitOfWork>(gardenContext);
	//################ DAL stuff ################


	//################ Controllers stuff ################
	//create the controllers
	//	creating the time service
	std::shared_ptr<TS::TimeService> timeService = std::make_shared<TS::TimeService>();
	timeService->initCurrentTime(myDate::my_clock::time_point{std::chrono::seconds{time(0)}});
	std::shared_ptr<sched::SchedulerService> scheduleService = std::make_shared<sched::SchedulerService>(timeService);

	//	creating the PlantsSuite, sprinklers and pump controllers
	hrdwrctrl::PlantsSuiteController plantsSuiteController{scheduleService, unitOfWork};
	std::shared_ptr<hrdwrctrl::SprinklersController> sprinklersController = std::make_shared<hrdwrctrl::SprinklersController>(unitOfWork, switchHub);
	std::shared_ptr<hrdwrctrl::PumpController> pumpController = std::make_shared<hrdwrctrl::PumpController>(unitOfWork, switchHub, 7);
	//################ Controllers stuff ################


	//################ Model stuff ################
	//create the model (not really using the repository add function (need to think on how to bypass the id on the add method (I think theoretically only on save we get the correct ids)))
	std::shared_ptr<garden::Garden> garden = unitOfWork->Garden().getById(-1);
	//	create the sprinklers
	std::shared_ptr<garden::Sprinkler> sprinkler1 = std::make_shared<garden::Sprinkler>();
	sprinkler1->id = 1;
	std::shared_ptr<garden::Sprinkler> sprinkler2 = std::make_shared<garden::Sprinkler>();
	sprinkler2->id = 2;
	std::shared_ptr<garden::Sprinkler> sprinkler3 = std::make_shared<garden::Sprinkler>();
	sprinkler3->id = 3;
	std::shared_ptr<garden::Sprinkler> sprinkler4 = std::make_shared<garden::Sprinkler>();
	sprinkler4->id = 4;
	//	adding the sprinklers to the model
	std::cout << std::endl << "#### Adding sprinklers to the garden" << std::endl;
	garden->_sprinklers.push_back(sprinkler1);//need to use the repository to achieve that.. not correct this way
	garden->_sprinklers.push_back(sprinkler2);
	garden->_sprinklers.push_back(sprinkler3);
	garden->_sprinklers.push_back(sprinkler4);

	//	creating program
	garden::Hour hour1{11,44};
	garden::Day day1;
	day1.hours.push_back(hour1);
	garden::TimePattern timePattern1;
	timePattern1.days.push_back(day1);
	auto program1 = std::make_shared<garden::SimpleProgram>();
	program1->id = 1;
	program1->timePattern = timePattern1;

	garden::Hour hour2{11,46};
	garden::Day day2;
	day2.hours.push_back(hour2);
	garden::TimePattern timePattern2;
	timePattern2.days.push_back(day2);
	auto program2 = std::make_shared<garden::SimpleProgram>();
	program2->id = 2;
	program2->timePattern = timePattern2;


	//	creating the plants
	std::shared_ptr<garden::Plant> plant1 = std::make_shared<garden::Plant>();
	plant1->id = 1;
	std::shared_ptr<garden::Plant> plant2 = std::make_shared<garden::Plant>();;
	plant2->id = 2;
	std::shared_ptr<garden::Plant> plant3 = std::make_shared<garden::Plant>();;
	plant3->id = 3;
	// adding the plants to the garden (here we can see that we need to do two things to add the plant (two things the repository should do))
	std::cout << std::endl << "#### Adding plants to the garden" << std::endl;
	plant1->garden = &(*garden);
	plant2->garden = &(*garden);
	plant3->garden = &(*garden);
	garden->_plants.push_back(plant1);
	garden->_plants.push_back(plant2);
	garden->_plants.push_back(plant3);

	//	add the sprinklers and the programs to the correct plants
	std::cout << std::endl << "#### Adding sprinkler to the corresponding plant" << std::endl;
	plant1->_sprinkler = sprinkler1;
	plant2->_sprinkler = sprinkler2;
	plant3->_sprinkler = sprinkler3;
	std::cout << std::endl << "#### Adding program to the corresponding plant" << std::endl;
	plant1->_program = program1;
	plant2->_program = program2;
	//################ Model stuff ################


	//The flow I'm expecting to see -
	//	- register the chipSelect to the SPI
	//	- register the switchArray to the chipSelect
	//	- create the DAL
	//	- create the controllers
	//		- plantSuite controller
	//		- plant controller
	//		- pump controller
	//	- create the model
	//		- create the sprinklers
	//			- the sprinkler controller should notify us that we add the sprinklers (maybe something with setting the status to of)
	//		- the pump controller should set the pump to off (i think)
	//	-



	printTime(timeService->getCurrentDateTime());
	scheduleService->execute();// running the scheduler
	std::cout << "Exiting the test####" << std::endl;
}

} /* namespace GardenModel */

#endif /* HARDWARETESTS_HARDWARETESTS_H_ */
