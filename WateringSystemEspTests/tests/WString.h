/*
 * WString.h
 *
 *  Created on: 12 בפבר׳ 2018
 *      Author: IsM
 */

#ifndef WSTRING_H_
#define WSTRING_H_

#include <iostream>
#include <time.h>

#define HIGH 1
#define LOW	 0

#define D1	 1
#define D2	 2
#define D3	 3

void digitalWrite(int pinId, int status){
	std::cout << "!!!!!!!!!!Setting pin ";
	std::cout <<  pinId;
	std::cout << "to ";
	std::cout <<  status << std::endl;
}



typedef std::string String;

class SerialClass {
public:
	void println(String str){
		std::cout << str << std::endl;
	}

	void print(String str){
		std::cout << str;
	}

	void print(const int& i){
		std::cout << i;
	}

	void println(const int& i){
		std::cout << i << std::endl;
	}
};

SerialClass Serial;

unsigned long startTime = time(0);

unsigned long millis() {
	return startTime - time(0);
}
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
