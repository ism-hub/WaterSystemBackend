/*
 * PropMapping.h
 *
 *  Created on: 6 áôáø× 2018
 *      Author: rina
 */

#ifndef DALMODULE_SERIALIZATIONSERVICE_PROPMAPPING_H_
#define DALMODULE_SERIALIZATIONSERVICE_PROPMAPPING_H_

namespace DAL {
struct NIL {
	typedef NIL Head;
	typedef NIL Tail;
};

template <typename Head, typename Types>
struct Lst
{
	typedef Head   head;
	typedef Lst<Types ...>   tail;
};

template <typename Head , typename Types = NIL>
struct Lst
{
	typedef Head   head;
	//typedef Lst<Types ...>   tail;
};



template <class type, const char* Key, class ... Children>
struct Property
{
	static const char* key = Key;
	typedef type   element_type;
	typedef Lst<Children ...>   children;

};




class PropMapping {
public:
	PropMapping();
	virtual ~PropMapping();
};

} /* namespace DAL */

#endif /* DALMODULE_SERIALIZATIONSERVICE_PROPMAPPING_H_ */
