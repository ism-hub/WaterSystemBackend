/*
 * Repository.h
 *
 *  Created on: 27 баев„ 2017
 *      Author: IsM
 */

#ifndef DAL_REPOSITORY_H_
#define DAL_REPOSITORY_H_

#include <vector>
#include <memory>

namespace garden {

template <class Entity>
class Repository {
public:
	Repository(){}
	virtual ~Repository(){}

	virtual std::shared_ptr<Entity> getById(int id) = 0;
	virtual std::vector<std::weak_ptr<Entity>> getAll() = 0;

	//virtual bool update(Entity& entity) = 0;

	virtual bool add(Entity&& entity) = 0;
	//bool addRange(std::vector<Entity> entities) = 0;

	virtual bool remove(int id) = 0;
	//bool remove(Entity entity) = 0;
	//bool removeRange(vstd::vector<Entity> entities) = 0;



};

} /* namespace DAL */

#endif /* DAL_REPOSITORY_H_ */
