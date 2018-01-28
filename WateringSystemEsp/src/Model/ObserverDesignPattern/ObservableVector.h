/*
 * ObservableVector.h
 *
 *  Created on: 4 срхїз 2017
 *      Author: IsM
 */

#ifndef MODEL_OBSERVERDESIGNPATTERN_OBSERVABLEVECTOR_H_
#define MODEL_OBSERVERDESIGNPATTERN_OBSERVABLEVECTOR_H_

#include <vector>
#include <ObserverDesignPattern/NotifyChangeSignalable.h>
#include <functional>

#if __cplusplus >= 201103L
#include <initializer_list>
#endif

namespace Model {
//why we using composition over inheritance (from std::vector (which also don't have virtual destractor)) -
//		https://stackoverflow.com/questions/6806173/subclass-inherit-standard-containers/7110262#7110262
//needs to notifies on adding or deleting an item, meaning any changes which not add or delete an item are ok
enum class Change { Added, Deleted };
template <class T>
class ObservableVector : public NotifyChangeSignalable<T, Change>{
public:

protected:
	std::vector<T> vector_;
private:
//	Signal<T, Change> on_change_;
//	Signal<T, Change> before_change_;
public:

	typedef std::function<void(T, Change)> ConnectorFncType ;

	ObservableVector(){
#ifdef DEBUG_MY_CODE
		Serial.println("ObservableVector CTOR");
#endif
	}

#if __cplusplus >= 201103L
	ObservableVector(std::initializer_list<typename std::vector<T>::value_type> __l, const typename std::vector<T>::allocator_type& __a = typename std::vector<T>::allocator_type())
    : vector_(__l, __a)
    {}
#endif

	virtual ~ObservableVector(){
#ifdef DEBUG_MY_CODE
		Serial.println("ObservableVector DTOR");
#endif

	}

	const std::vector<T>& getInnerVector() const{
		return vector_;
	}

	void push_back(const T& __x) {
		NotifyChangeSignalable<T, Change>::before_change().emit(__x, Change::Added);
		vector_.push_back(__x);
		NotifyChangeSignalable<T, Change>::on_change().emit(__x, Change::Added);
	}

	void push_back(T&& __x){
		NotifyChangeSignalable<T, Change>::before_change().emit(__x, Change::Added);
		vector_.push_back(__x);//std::forward?
		NotifyChangeSignalable<T, Change>::on_change().emit(__x, Change::Added);
	}

	/*template<typename _Tp, typename _Alloc>
	typename std::vector<_Tp, _Alloc>::iterator erase(
			typename std::vector<_Tp, _Alloc>::iterator __position) {
		before_change_.emit(*__position, Change::Deleted);
		vector_(__position);
		on_change_(*__position, Change::Deleted);
	}*/

	/*template<typename _Tp, typename _Alloc>
	typename std::vector<_Tp, _Alloc>::iterator erase(
			std::vector<_Tp, _Alloc>::iterator __first,
			std::vector<_Tp, _Alloc>::iterator __last) {
		before_change_.emit(*__position, Deleted);
		vector_(__first, __last);
	}*/

	// assigns a new value to this Property
	virtual ObservableVector<T>& operator=(std::vector<T> const& rhs) {
		for (auto t : rhs) { //TODO: check if can do it faster using &
			push_back(t);
		}
		return *this;
	}

	/*virtual ObservableVector<T>& operator=(std::vector<T>&& rhs) {
		for (auto& t : rhs) {
			push_back(std::move(t));
		}
		return *this;
	}*/
};

} /* namespace Model */

#endif /* MODEL_OBSERVERDESIGNPATTERN_OBSERVABLEVECTOR_H_ */
