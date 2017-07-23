/*
 * WateringSystemApiParser.h
 *
 *  Created on: 11 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_WATERINGSYSTEMAPIPARSER_H_
#define HTTP_WATERINGSYSTEMAPIPARSER_H_
/*
#include <map>
#include <vector>
#include <WString.h>
#include "HttpServletRequest.h"

using namespace std;

namespace Http {

// The list of APi uri's
//	- Sprinklers
//	- Programs
//	- Sprinklers/{id}
//	- Programs/{id}
//	- Sprinklers/{id}/Programs
//	- Programs/{id}/Sprinklers

class WateringSystemApiParser {

public:

	WateringSystemApiParser() {

	}
	virtual ~WateringSystemApiParser() {

	}

	ParsedRequest parse(HttpServletRequest& request){
		ParsedRequest parsedRequest;

		vector<String>* uriTokens = getLisOfTokens(request.url);
		parsedRequest.uriType = getUriType(*uriTokens);
		parsedRequest.httpMethod = request.httPMethod;
		parsedRequest.VarValuePairs = getRequestData(*uriTokens, parsedRequest.uriType, request);
		return parsedRequest;
	}

protected:

	std::map<String, String>* getRequestData(vector<String>& uriTokens, ApiUriType uriType, HttpServletRequest& request) {
		std::map<String,String>* reqDataDic = new std::map<String,String>();

		//get data from uri
		if(uriType == SPRINKLERS_ID || uriType == SPRINKLERS_ID_PROGRAMS || uriType == PROGRAMS_ID || uriType == PROGRAMS_ID_SPRINKLERS)
			reqDataDic->insert(pair<String, String>("id", uriTokens[1]));

		//get data from body (its the json string)
		reqDataDic->insert(pair<String, String>("body", request.requestBody));

		return reqDataDic;
	}

	ApiUriType getUriType(vector<String>& tokens) {
		bool correctUriFlag = true;
		ApiUriType type;

		if (tokens[0] == "Sprinklers") {
			if (tokens.size() == 1)
				type = SPRINKLERS;
			else if (tokens.size() == 2)
				type = SPRINKLERS_ID;
			else if (tokens.size() == 3) {
				correctUriFlag = tokens[2] == "Programs";
				type = SPRINKLERS_ID_PROGRAMS;
			}
		} else if (tokens[0] == "Programs") {
			if (tokens.size() == 1)
				type = PROGRAMS;
			else if (tokens.size() == 2)
				type = PROGRAMS_ID;
			else if (tokens.size() == 3) {
				type = PROGRAMS_ID_SPRINKLERS;
				correctUriFlag = tokens[2] == "Sprinklers";
			}
		} else
			correctUriFlag = false;

		if (!correctUriFlag)
			return UNKNOWN;//throw "Invalid Uri"; //apparently exceptions arn't supported

		return type;
	}

	vector<String>* getLisOfTokens(String uri) {
		vector<String>* tokens = new vector<String>();
		int nextTokenStartPlace = 0;

		int indx = uri.indexOf('/', nextTokenStartPlace);
		while (indx != -1) {
			tokens->push_back(uri.substring(nextTokenStartPlace, indx));
			nextTokenStartPlace = indx;
		}
		tokens->push_back(uri.substring(nextTokenStartPlace));
		return tokens;
	}


};

} /* namespace Http */

#endif /* HTTP_WATERINGSYSTEMAPIPARSER_H_ */

