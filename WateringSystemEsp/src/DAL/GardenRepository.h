/*
 * GardenRepository.h
 *
 *  Created on: 26 αιεμι 2017
 *      Author: IsM
 */

#ifndef DAL_GARDENREPOSITORY_H_
#define DAL_GARDENREPOSITORY_H_

#include <GardenModelContext.h>

namespace DAL {

class GardenRepository {
public:
	GardenModelContext& _gardenModelContext;
	GardenRepository(GardenModelContext& gardenModelContext) : _gardenModelContext(gardenModelContext){

	}
	virtual ~GardenRepository() {

	}
};

} /* namespace Http */

#endif /* DAL_GARDENREPOSITORY_H_ */
