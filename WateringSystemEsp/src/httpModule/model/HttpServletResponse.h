/*
 * HttpServletResponse.h
 *
 *  Created on: 9 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_HTTPSERVLETRESPONSE_H_
#define HTTP_HTTPSERVLETRESPONSE_H_

#include <WString.h>
#include <HttpEnums.h>
#include <WString.h>

#include <HardwareSerial.h>

namespace Http {

enum HttpCode {SC_OK=200, SC_NOT_FOUND = 404, SC_ACCEPTED = 202, SC_NOT_IMPL = 501};


class HttpServletResponse {
public:
	static const String CONTENT_TYPE_JSON;// = "application/json";
	String content_type;
	String content;
	HttpCode _httpCode;
	HttpServletResponse() :
			content_type(""), content(""), _httpCode(SC_NOT_IMPL) {

		}
	HttpServletResponse(String contentType, String content, HttpCode httpCode) :
		content_type(contentType), content(content), _httpCode(httpCode) {

	}
	virtual ~HttpServletResponse() {
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ HttpServletResponse DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}
};

//const String HttpServletResponse::CONTENT_TYPE_JSON = "application/json";
} /* namespace Http */

#endif /* HTTP_HTTPSERVLETRESPONSE_H_ */
