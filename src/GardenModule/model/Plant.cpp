/*
 * Plant.cpp
 *
 *  Created on: 16 בפבר׳ 2018
 *      Author: IsM
 */

#include <GardenModule/model/Garden.h>
#include <GardenModule/model/Plant.h>


void garden::Plant::setProgram(int pid){
	_program = garden->getProgram(pid);
}

void garden::Plant::setSprinkler(int sid){
	_sprinkler = garden->getSprinkler(sid);
}
