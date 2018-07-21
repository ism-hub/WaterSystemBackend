/*
 * IGardenModelContex.h
 *
 *  Created on: 13 במרץ 2018
 *      Author: rina
 */

#ifndef DALMODULE_REPOSITORYPATTERN_IGARDENMODELCONTEX_H_
#define DALMODULE_REPOSITORYPATTERN_IGARDENMODELCONTEX_H_

#include <memory>
#include <Garden.h>

namespace DAL {

class IGardenModelContex {
public:
	IGardenModelContex() {}
	virtual ~IGardenModelContex() {}

	virtual std::shared_ptr<GardenModel::Garden> getGarden()= 0;

	virtual bool init() = 0;

	virtual bool saveChanges() = 0;

};

} /* namespace DAL */

#endif /* DALMODULE_REPOSITORYPATTERN_IGARDENMODELCONTEX_H_ */
