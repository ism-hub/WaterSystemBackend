/*
 * FS.h
 *
 *  Created on: 12 במרץ 2018
 *      Author: rina
 */

#ifndef HARDWARETESTS_FS_H_
#define HARDWARETESTS_FS_H_

#include <stdio.h>

class File{
public:
	const char* location;
	const char* premission;
	FILE* fp = NULL;

	File(const char* location, const char* premission) : location(location), premission(premission){
		fp = fopen(location, premission);
	}

	void print(String str) {
		fputs(str.c_str(), fp);
		// fclose(fp);
	}

	void flush(){
	}

	void close(){
		fclose(fp);
	}

	bool operator!(){
		return !fp;
	}

	String readStringUntil(char ch='\0') {
		char buff[4000];
		fgets(buff, 4000, fp);
		return String{buff};
	}
};

class SPIFFSC {
public:
	bool begin(){
		return true;
	}

	bool end(){
		return true;
	}

	File open(const char* location, const char* premission){
		return File(location, premission);
		// return *fopen(location, premission);
	}

};

SPIFFSC SPIFFS;



#endif /* HARDWARETESTS_FS_H_ */
