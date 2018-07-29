/*
 * AcceptableModel.h
 *
 *  Created on: 28 αιεμι 2018
 *      Author: IsM
 */

#ifndef HTTPMODULE_DISPATCHER_ACCEPTABLEMODEL_H_
#define HTTPMODULE_DISPATCHER_ACCEPTABLEMODEL_H_

#include <memory>
namespace Http {

template <class Model>
class AcceptableModel {
public:
	AcceptableModel() {}
	virtual ~AcceptableModel() {}

	template <class Visitor>
	std::shared_ptr<void> accept(Visitor& visitor){
		return visitor.template visit<Model>(*this);
	}
};

} /* namespace Http */

#endif /* HTTPMODULE_DISPATCHER_ACCEPTABLEMODEL_H_ */
