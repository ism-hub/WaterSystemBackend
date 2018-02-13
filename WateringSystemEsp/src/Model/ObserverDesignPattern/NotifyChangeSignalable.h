/*
 * Signalable.h
 *
 *  Created on: 4 срхїз 2017
 *      Author: IsM
 */

#ifndef MODEL_OBSERVERDESIGNPATTERN_NOTIFYCHANGESIGNALABLE_H_
#define MODEL_OBSERVERDESIGNPATTERN_NOTIFYCHANGESIGNALABLE_H_

#include <ObserverDesignPattern/Signal.hpp>

namespace Model {

//publisher
// for objects which can publish (signal) changes for their observers (slots)
template<class... Args>
class NotifyChangeSignalable {
protected:
	Signal<Args ...> on_change_;
	Signal<Args ...> before_change_;

public:
	NotifyChangeSignalable(){}
	virtual ~NotifyChangeSignalable(){
#ifdef DEBUG_MY_CODE
		Serial.println("NotifyChangeSignalable DTOR");
#endif

	}

	// returns a Signal which is fired when the internal value
	// will be changed. The old value is passed as parameter.
	virtual Signal<Args ...> const& before_change() const {
		return before_change_;
	}

	// returns a Signal which is fired when the internal value
	// has been changed. The new value is passed as parameter.
	virtual Signal<Args ...> const& on_change() const {
		return on_change_;
	}

	// if there are any Properties connected to this Property,
	// they won't be notified of any further changes
	virtual void disconnect_auditors() {
		on_change_.disconnect_all();
		before_change_.disconnect_all();
	}


};

} /* namespace Model */

#endif /* MODEL_OBSERVERDESIGNPATTERN_NOTIFYCHANGESIGNALABLE_H_ */
