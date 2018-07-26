/*
 * HardwareModule.h
 *
 *  Created on: 6 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWAREMODULE_HARDWAREMODULE_H_
#define HARDWAREMODULE_HARDWAREMODULE_H_

#include <vector>
#include <memory>
#include <ModuleFramework/Module.h>
#include <ModuleFramework/Container/Container.h>
#include <IService.h>
#include <HardwareModule/controllers/PlantsSuiteController.h>
#include <HardwareModule/controllers/PumpController.h>
#include <HardwareModule/controllers/SprinklersController.h>
#include <SPIService.h>

//will contain all controllers which connects the model to the hardware
namespace hrdwrmodule {


class PlantSuiteControllerIService : public sfwk::IService {

	std::shared_ptr<hrdwrctrl::PlantsSuiteController> plantSuiteController = nullptr;
	std::shared_ptr<sched::SchedulerService> _scheduleService;
	std::shared_ptr<DAL::GardenUnitOfWork> _unitOfWork;
public:

		PlantSuiteControllerIService(std::shared_ptr<sched::SchedulerService> scheduleService, std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork):
			sfwk::IService("PlantSuiteControllerIService"),
			_scheduleService(scheduleService),
			_unitOfWork(unitOfWork)
			{}

		virtual ~PlantSuiteControllerIService() {
			StopService();
		}

		virtual int StartService() {
			plantSuiteController = std::make_shared<hrdwrctrl::PlantsSuiteController>(_scheduleService, _unitOfWork);
			return 1;
		}

		virtual int StopService() {
			plantSuiteController = nullptr;//releasing the pointer, it not really stopping the service, but no one should save pointer to it
			return 1;
		}

};

class PumpControllerIService : public sfwk::IService {

	std::shared_ptr<hrdwrctrl::PumpController> pumpController = nullptr;
	std::shared_ptr<DAL::GardenUnitOfWork> _unitOfWork;
	std::shared_ptr<hrdwrctrl::SwitchArray> _switchHub;
	int _pumpSwitchNumber;
public:

	PumpControllerIService(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork, std::shared_ptr<hrdwrctrl::SwitchArray> switchHub, int pumpSwitchNumber):
			sfwk::IService("PumpControllerIService"),
			_unitOfWork(unitOfWork),
			_switchHub(switchHub),
			_pumpSwitchNumber(pumpSwitchNumber)
			{}

		virtual ~PumpControllerIService() {
			StopService();
		}

		virtual int StartService() {
			pumpController = std::make_shared<hrdwrctrl::PumpController>(_unitOfWork, _switchHub, _pumpSwitchNumber);
			return 1;
		}

		virtual int StopService() {
			pumpController = nullptr;//releasing the pointer, it not really stopping the service, but no one should save pointer to it
			return 1;
		}

};

class SprinklersControllerIService : public sfwk::IService {

	std::shared_ptr<hrdwrctrl::SprinklersController> sprinklersController = nullptr;
	std::shared_ptr<DAL::GardenUnitOfWork> _unitOfWork;
	std::shared_ptr<hrdwrctrl::SwitchArray> _switchHub;
public:

	SprinklersControllerIService(std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork, std::shared_ptr<hrdwrctrl::SwitchArray> switchHub):
			sfwk::IService("SprinklersControllerIService"),
			_unitOfWork(unitOfWork),
			_switchHub(switchHub)
			{}

		virtual ~SprinklersControllerIService() {
			StopService();
		}

		virtual int StartService() {
			sprinklersController = std::make_shared<hrdwrctrl::SprinklersController>(_unitOfWork, _switchHub);
			return 1;
		}

		virtual int StopService() {
			sprinklersController = nullptr;//releasing the pointer, it not really stopping the service, but no one should save pointer to it
			return 1;
		}

};

class SPIServiceIService : public sfwk::IService{
	std::shared_ptr<hrdwrctrl::SPIService> spiService = nullptr;
public:

	SPIServiceIService(std::shared_ptr<hrdwrctrl::SPIService> spiService):
		sfwk::IService("SPIServiceIService"),
		spiService(spiService)
	{}

		virtual ~SPIServiceIService() {
			StopService();
		}

		virtual int StartService() {
			//if(spiService == nullptr){
			//	spiService = std::make_shared<hrdwrctrl::SPIService>(numberOfPins);
			//	spiService->unselectAllChips();
			//}
			return 1;
		}

		virtual int StopService() {
			//spiService = nullptr;//releasing the pointer, it not really stopping the service, but no one should save pointer to it
			return 1;
		}
};

std::shared_ptr<hrdwrctrl::SPIService> SPIServiceCreator(){
	std::shared_ptr<hrdwrctrl::SPIService> spiService = std::make_shared<hrdwrctrl::SPIService>(8);
	Serial.println("Unselecting all chips (SPIService)");
	spiService->unselectAllChips();
	return spiService;//TODO: add to configuration
}

/*std::shared_ptr<hrdwrctrl::ChipSelect> ChipSelectCreator(std::shared_ptr<hrdwrctrl::SPIService> spiService){
	hrdwrctrl::chip chipSelectSpiConf{"ChipSelect", 2};
	hrdwrctrl::ChipSelect::ChipSelectConfig chipSelectConf = hrdwrctrl::ChipSelect::ChipSelectConfig(8, chipSelectSpiConf);
	return std::make_shared<hrdwrctrl::ChipSelect>(spiService, chipSelectConf);
}

std::shared_ptr<hrdwrctrl::chipSelectSPIService> ChipSelectISPIServiceCreator(std::shared_ptr<hrdwrctrl::ChipSelect> chipSelect) {
	return std::make_shared<hrdwrctrl::chipSelectSPIService>(chipSelect);
}*/

std::shared_ptr<hrdwrctrl::SwitchArray> SwitchArrayCreator(std::shared_ptr<hrdwrctrl::SPIService> spiService){
	unsigned int csPin = 2;//to what chip select pin we connected to
	unsigned int switchArraySize = 8;
	hrdwrctrl::SwitchArray::SwitchArrayConfig config(SPISettings(500, MSBFIRST, SPI_MODE0), csPin, switchArraySize);
	std::shared_ptr<hrdwrctrl::SwitchArray> switchArray =std::make_shared<hrdwrctrl::SwitchArray>(spiService, config);//notice - we gave it the chipSelectSPIService cause it on the chipSelect chip
	//reset switch array
	Serial.println("Reseting switch array");
	switchArray->resetState();
	return switchArray;
}

class HardwareModule  : public MF::ModuleBase {

public:
	HardwareModule() {}
	virtual ~HardwareModule() {}

	void start(std::shared_ptr<cntnr::Container> container){

		container->registerType<hrdwrctrl::SPIService>(&SPIServiceCreator);
		container->registerType<hrdwrctrl::SwitchArray>(&SwitchArrayCreator);

		std::shared_ptr<hrdwrctrl::SPIService> spiService = container->resolve<hrdwrctrl::SPIService>();
		std::shared_ptr<hrdwrctrl::SwitchArray> switchArray = container->resolve<hrdwrctrl::SwitchArray>();

		std::shared_ptr<sched::SchedulerService> scheduleService = container->resolve<sched::SchedulerService>();
		std::shared_ptr<DAL::GardenUnitOfWork> unitOfWork = container->resolve<DAL::GardenUnitOfWork>();

		auto plantsSuitControllerIService = std::make_shared<PlantSuiteControllerIService>(scheduleService, unitOfWork);
		auto pumpControllerIService = std::make_shared<PumpControllerIService>(unitOfWork, switchArray, 7);
		auto sprinklersControllerIService = std::make_shared<SprinklersControllerIService>(unitOfWork, switchArray);
		auto spiServiceIService = std::make_shared<SPIServiceIService>(spiService);

		//By adding the services to the ServiceFrameWork instance we are making sure that the controllers will stay alive for the duration of the program exe
		std::shared_ptr<sfwk::ServiceFrameWork> serviceFramework = container->resolve<sfwk::ServiceFrameWork>();
		serviceFramework->AddService(plantsSuitControllerIService);
		serviceFramework->AddService(pumpControllerIService);
		serviceFramework->AddService(sprinklersControllerIService);
		serviceFramework->AddService(spiServiceIService);

		plantsSuitControllerIService->StartService();
		pumpControllerIService->StartService();
		sprinklersControllerIService->StartService();


	}

};

} /* namespace hrdwrmodule */

#endif /* HARDWAREMODULE_HARDWAREMODULE_H_ */
