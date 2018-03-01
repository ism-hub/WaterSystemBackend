/*
 * WString.h
 *
 *  Created on: 12 בפבר׳ 2018
 *      Author: IsM
 */

#ifndef WSTRING_H_
#define WSTRING_H_

#include <iostream>

typedef std::string String;

class SerialClass {
public:
	void println(String str){
		std::cout << str << std::endl;
	}
};

SerialClass Serial;
/*class String : public std::string {
public:
	String() : std::string() {}
	String(const char* str) : std::string(str) {}
	String(char* str) : std::string(str) {}
	//String(std::string str) : std::string(str) {}
	String(const std::string str) : std::string(str) {}
	//String(std::string& str) : std::string(str) {}


	String  operator =(const String &rhs) {return std::string::operator =(rhs);}
	String  operator =(const char *cstr) {return std::string::operator =(cstr);}

	int compareTo(const String &s) const {return std::string::compare(s);}
	unsigned char equals(const String &s) const {return (std::string)(s) == (std::string)(*this);}
	unsigned char equals(const char *cstr) const {return (std::string)(*this) == (cstr); }
	unsigned char operator ==(const String &rhs) const {
		return equals(rhs);
	}
	unsigned char operator ==(const char *cstr) const {
		return equals(cstr);
	}
	unsigned char operator !=(const String &rhs) const {
		return !equals(rhs);
	}
	unsigned char operator !=(const char *cstr) const {
		return !equals(cstr);
	}


	virtual ~String(){}
};*/

#endif /* WSTRING_H_ */
