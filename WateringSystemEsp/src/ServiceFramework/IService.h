/*
 * IService.h
 *
 *  Created on: 5 במרץ 2018
 *      Author: IsM
 */

#ifndef SERVICEFRAMEWORK_ISERVICE_H_
#define SERVICEFRAMEWORK_ISERVICE_H_

#include <WString.h>
#include <vector>

namespace sfwk {

class IService {
	const String _id;
	std::vector<String> _dependencies;
public:

	IService(const String id): _id(id){}

	const String getId(){return _id;}
	void AddDependency (String id){_dependencies.push_back(id);}

	virtual int StartService() = 0;
	virtual int RestartService() = 0;
	virtual int StopService() = 0;

	virtual ~IService() {}
};

} /* namespace sfwk */

#endif /* SERVICEFRAMEWORK_ISERVICE_H_ */
