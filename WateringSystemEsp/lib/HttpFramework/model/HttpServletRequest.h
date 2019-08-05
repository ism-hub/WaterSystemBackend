/*
 * HttpServletRequest.h
 *
 *  Created on: 9 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_HTTPSERVLETREQUEST_H_
#define HTTP_HTTPSERVLETREQUEST_H_

#include <WString.h>
#include <HttpFramework/model/HttpEnums.h>
#include <map>
#include <vector>
#include <stdio.h>

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
	std::vector<String> urlTokens;

	HttpServletRequest(String requestBody, Http::HTTPMethod httPMethod, String url) :
			requestBody(requestBody), httPMethod(httPMethod), url(url) {
	//	Serial.println ( "HttpServletRequest CTOR" );
		addURLTokens(url);
//		Serial.println("url:" + url);
//		Serial.print("size:");
//		Serial.println(urlTokens.size());
	}

	~HttpServletRequest() {
	}

private:
	void addURLTokens(String url) {
		if(url.length() == 0 || url.length() == 1)
			return;
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
