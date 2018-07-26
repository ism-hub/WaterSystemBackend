/*
 * Error.h
 *
 *  Created on: 26 αιεμι 2018
 *      Author: IsM
 */

#ifndef ERRORHANDLING_ERROR_H_
#define ERRORHANDLING_ERROR_H_

#include <WString.h>
#include <map>
#include <HardwareSerial.h>



namespace err {

enum class ErrorCode {NO_ERROR,
//SPIService errors
	ERR_SPI_CS_INVALID,
	ERR_SPI_CS_TAKEN,
	ERR_SPI_CS_ALREADYFREE,
	ERR_SPI_FAILED_SENDDATA
};

template <class T>
const char* compiletimeTypeid(T){
	return __PRETTY_FUNCTION__;
}

template <class ValueType>
class Error {
public:
	ErrorCode errorCode = ErrorCode::NO_ERROR;
	String details = "";//more details about the error
	ValueType value;
	typedef ValueType valueType;
public:
	Error(ErrorCode errorCode, String details = "") : errorCode(errorCode), details(details)
	{}

	Error(ValueType value) :errorCode(ErrorCode::NO_ERROR), details(""), value(value)
	{}

	//need to use it only when we have an error
	template <class ErrorValueType>
	Error(const Error<ErrorValueType>& err2) : errorCode(err2.errorCode), details(err2.details)
	{
		if(err2.errorCode == ErrorCode::NO_ERROR){
			Serial.println("__ERROR: wrong use of Error() copy constructor class");
			Serial.println(compiletimeTypeid(*this));
			Serial.println(compiletimeTypeid(err2));
		}
	}

	template<ValueType>
	Error(const Error<ValueType>& err) : errorCode(err.errorCode), details(err.details), value(err.value)
	{
		Serial.println("copy ctor of same types of Error");
	}

	virtual ~Error(){}

	bool operator ==(const ErrorCode errorCode) const{
		return this->errorCode == errorCode;
	}

	bool operator !=(const ErrorCode errorCode) const{
		return this->errorCode != errorCode;
	}

	String errorCodeToString(){
		return errorCodeToString(errorCode);
	}

	static String errorCodeToString(const ErrorCode errorCode){
		switch(errorCode){
		case ErrorCode::NO_ERROR: return "NO_ERROR";
		case ErrorCode::ERR_SPI_CS_INVALID: return "ERR_SPI_CS_INVALID";
		case ErrorCode::ERR_SPI_CS_TAKEN: return "ERR_SPI_CS_TAKEN";
		case ErrorCode::ERR_SPI_CS_ALREADYFREE: return "ERR_SPI_CS_ALREADYFREE";
		case ErrorCode::ERR_SPI_FAILED_SENDDATA: return "ERR_SPI_FAILED_SENDDATA";
		default:
			return "{MISSING ERROR TO STRING CONVERSION IN Error::errorCodeToString}";
		}
	}

public:
	//mapping error codes to their default msg
	//static const std::map<ErrorCode, String> ErrorCodeToMsg;
};

} /* namespace hrdwrctrl */

#endif /* ERRORHANDLING_ERROR_H_ */
