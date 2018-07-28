/*
 * IModelContex.h
 *
 *  Created on: 27 αιεμι 2018
 *      Author: IsM
 */

#ifndef DALFRAMEWORK_IMODELCONTEX_H_
#define DALFRAMEWORK_IMODELCONTEX_H_

#include <memory>

namespace dalfw {

template <class ModelType>
class IModelContex {
public:
	IModelContex() {}
	virtual ~IModelContex() {}

	/*
	 * On the first call - if exists on the data store reads it and serialize it, else returns a default model
	 * Not first call - gives what is already in memory (without going to the data store)
	 * */
	virtual std::shared_ptr<ModelType> get() = 0;

	/*
	 * saves the changes of the model to the data store
	 * */
	virtual bool save() = 0;

	/*
	 * force going to the data store to take fresh copy
	 * */
	virtual bool init() = 0;
};

} /* namespace dalfw */

#endif /* DALFRAMEWORK_IMODELCONTEX_H_ */
