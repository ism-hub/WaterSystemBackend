/*
 * HttpServletRequest.h
 *
 *  Created on: 9 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_HTTPSERVLETREQUEST_H_
#define HTTP_HTTPSERVLETREQUEST_H_

#include <WString.h>
#include <HttpEnums.h>
#include <map>
#include <vector>
#include <stdio.h>

using namespace std;

namespace Http {

enum ApiUriType {
	UNKNOWN,
	SPRINKLERS,
	PROGRAMS,
	SPRINKLERS_ID,
	PROGRAMS_ID,
	SPRINKLERS_ID_PROGRAMS,
	PROGRAMS_ID_SPRINKLERS
};

struct ParsedRequest {
	ApiUriType uriType;
	HTTPMethod httpMethod;
	std::map<String, String>* VarValuePairs; //check to see if the map is getting destroyed when the parsed uri is.
};

class HttpServletRequest {
public:
	String requestBody;
	Http::HTTPMethod httPMethod;
	String url;
	vector<String> urlTokens;

	HttpServletRequest(String requestBody, Http::HTTPMethod httPMethod, String url) :
			requestBody(requestBody), httPMethod(httPMethod), url(url) {
		Serial.println ( "in the HttpServletRequest CTOR" );
		addURLTokens(url);
	}

	virtual ~HttpServletRequest() {
		Serial.println("$#$##$#$#$#$##$$##$#$#$#$#$#$#$#$ HttpServletRequest DESTRACTOR has been called ##$#$#$#$#$$##$#$#$#$$##$#$#$#$");
	}

private:
	void addURLTokens(String url) {
		int nextTokenStartPlace = 1;
		int indx = url.indexOf('/', nextTokenStartPlace);
		while (indx != -1) {
			String tok = url.substring(nextTokenStartPlace, indx);
			urlTokens.push_back(tok);
			nextTokenStartPlace = indx + 1;
			indx = url.indexOf('/', nextTokenStartPlace);
		}
		urlTokens.push_back(url.substring(nextTokenStartPlace));
	}
};

} /* namespace Http */

#endif /* HTTP_HTTPSERVLETREQUEST_H_ */
