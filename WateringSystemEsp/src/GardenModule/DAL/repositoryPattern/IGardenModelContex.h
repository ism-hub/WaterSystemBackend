/*
 * IGardenModelContex.h
 *
 *  Created on: 13 במרץ 2018
 *      Author: rina
 */

#ifndef DALMODULE_REPOSITORYPATTERN_IGARDENMODELCONTEX_H_
#define DALMODULE_REPOSITORYPATTERN_IGARDENMODELCONTEX_H_

#include <memory>
#include <GardenModule/model/Garden.h>

namespace garden {

class IGardenModelContex {
public:
	IGardenModelContex() {}
	virtual ~IGardenModelContex() {}

	virtual std::shared_ptr<garden::Garden> get()= 0;

	virtual bool init() = 0;

	virtual bool save() = 0;

};

} /* namespace DAL */

#endif /* DALMODULE_REPOSITORYPATTERN_IGARDENMODELCONTEX_H_ */
