/*
 * JsonSerializationVisitor.h
 *
 *  Created on: 28 αιεμι 2018
 *      Author: IsM
 */

#ifndef HTTPMODULE_INTERCEPTORS_SERIALIZATIONVISITOR_H_
#define HTTPMODULE_INTERCEPTORS_SERIALIZATIONVISITOR_H_

#include <memory>

namespace Http {

template <class Serializer>
class SerializationVisitor {
	std::shared_ptr<Serializer> serializer = nullptr;
public:
	SerializationVisitor(std::shared_ptr<Serializer> serializer) : serializer(serializer) {}
	virtual ~SerializationVisitor() {}

	template <class Model>
	std::shared_ptr<void> visit(Model& model) {
		std::shared_ptr<String> serialized = std::make_shared<String>();
		serializer->serialize(model, *serialized);
		return serialized;
	}
};

} /* namespace Http */

#endif /* HTTPMODULE_INTERCEPTORS_SERIALIZATIONVISITOR_H_ */
