/*
 * stuff.h
 *
 *  Created on: 31 ����� 2018
 *      Author: IsM
 */

#ifndef STUFF_STUFF_H_
#define STUFF_STUFF_H_

#include <chrono> 
#include <functional>

namespace stf {


class Link{
	std::vector<String> tokenizedHref;

	std::vector<String> ssplit2(const char *str, char c = '/')
	{
		std::vector<String> result;
		String string(str);
		const char* start = str;
		do
		{
			const char *begin = str;
			while(*str != c && *str)
				str++;
			result.push_back(string.substring(begin - start, str - start));
		} while (0 != *str++);

		return result;
	}
	/*std::vector<String> ssplit2(const char *str, char c = '/')
	{
		std::vector<String> result;
		do
		{
			const char *begin = str;
			while(*str != c && *str)
				str++;
			result.push_back(String(begin, str));
		} while (0 != *str++);

		return result;
	}*/

public:
	String rel;
	String href;

	Link(const String& rel, const String& href): rel(rel), href(href){}
	Link() {}

	const std::vector<String>& getHrefTokens(){//TODO: 111 After finishing the rest of the service: this code is suppose to do the same as the uri parser in the HttpServletRequest, extract that code to some lib
		if(tokenizedHref.size() > 0)//already tokenized
			return tokenizedHref;
		tokenizedHref = ssplit2(href.c_str(), '/');
		return tokenizedHref;
	}

	template <class MappingFile, class Archive>
	void save(MappingFile& mappingFile, Archive& archive) const{
		archive.addProperties(mappingFile, MACRO_NVP(rel), MACRO_NVP(href));
	}

	template <class MappingFile, class Archive>
	void load(MappingFile& mappingFile, Archive& archive) {
		archive.loadProperties(mappingFile, MACRO_NVP(rel), MACRO_NVP(href));
	}
};

/*
 * if the
 * */
template <class T>
class CachedValue {
	T t;
	std::chrono::seconds interval;
	std::function<void(T&)> updateFnc;
	std::chrono::seconds lastUpdate;
	bool isUpdated = false; //so we will know when the first update happens
public:
	CachedValue(std::chrono::seconds interval, std::function<void(T&)> updateFnc) : interval(interval), updateFnc(updateFnc){
	//	Serial.println("-------------------------- CachedValue CTOR");
	}

	 ~CachedValue(){
//		Serial.println("-------------------------- CachedValue DTOR");
	}

	T& get() {
		if((timeNow() - lastUpdate > interval ) || (!isUpdated)){
			t.clear();
			updateFnc(t);
			lastUpdate = timeNow();
			isUpdated = true;
		}
		return t;
	}

	T& forceUpdate() {
		t.clear();
		updateFnc(t);
		isUpdated = true;
		lastUpdate = timeNow();
		return t;
	}

protected:
	std::chrono::seconds timeNow() {
		unsigned long int secs = millis() / 1000;
		return std::chrono::seconds{secs};
	}
};


} /* namespace stf */

#endif /* STUFF_STUFF_H_ */
