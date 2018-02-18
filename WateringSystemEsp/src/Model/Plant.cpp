/*
 * Plant.cpp
 *
 *  Created on: 16 בפבר׳ 2018
 *      Author: IsM
 */

#include <Garden.h>
#include <Plant.h>


void GardenModel::Plant::setProgram(int pid){
	_program = garden->getProgram(pid);
}

void GardenModel::Plant::setSprinkler(int sid){
	_sprinkler = garden->getSprinkler(sid);
}
