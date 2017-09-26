/*
 * utils.h
 *
 *  Created on: 21 בספט׳ 2017
 *      Author: IsM
 */

#ifndef MODULEFRAMEWORK_UTILS_H_
#define MODULEFRAMEWORK_UTILS_H_


namespace MF {

template <class T>
const char* compiletimeTypeid(){
	return __PRETTY_FUNCTION__;
}

template <class T>
const char* getTemplateName(){ //... [with type = int]
	const char* stratOfTypePointer = __PRETTY_FUNCTION__;
	while(*stratOfTypePointer != '=')
		stratOfTypePointer++;

	stratOfTypePointer +=2;

	const char* endOfTypePointer = stratOfTypePointer;
	while (*endOfTypePointer != ']')
		endOfTypePointer++;

	endOfTypePointer--;

	const char* stratOfTypeWithoutNamespaces = endOfTypePointer;
	while (stratOfTypeWithoutNamespaces >= stratOfTypePointer && !(*(stratOfTypeWithoutNamespaces - 1) == ':' && *(stratOfTypeWithoutNamespaces - 2) == ':'))//while we didnt see "::"
		stratOfTypeWithoutNamespaces--;

	int length = endOfTypePointer - stratOfTypeWithoutNamespaces + 2;
	char* typeName = new char[length];
	typeName[length - 1] = '\0';

	for(int i = 0; i < length - 2; i++)
		typeName[i] = stratOfTypeWithoutNamespaces[i];

	return typeName;
}

/*class Serial{
public:
	void println(const char* str){
		std::cout << str << std::endl;
	}
};

Serial Serial;*/

} /* namespace moduleFramework */

#endif /* MODULEFRAMEWORK_UTILS_H_ */
