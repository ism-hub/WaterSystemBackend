/*
 * MappingHandler.h
 *
 *  Created on: 10 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_MAPPINGHANDLER_H_
#define HTTP_MAPPINGHANDLER_H_

namespace Http {

class MappingHandler {
public:
	MappingHandler(){

	}

	virtual ~MappingHandler(){
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ MappingHandler DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

	//think on how you design this, think if
	//	- the approach of HttpRequest in and HttpResponse out is a must (ofc it must) so how to implement it in the best way
	//	-
	/*ExecutionChain& Map(){

	}*/
};

} /* namespace Http */

#endif /* HTTP_MAPPINGHANDLER_H_ */
