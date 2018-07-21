/**
 * from: http://simmesimme.github.io/tutorials/2015/09/21/properties
 */

#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include <ObserverDesignPattern/Signal.hpp>
#include <ObserverDesignPattern/NotifyChangeSignalable.h>

//#include <iostream>

namespace Model {
// A Property is a encpsulates a value and may inform
// you on any changes applied to this value.

template <typename T>
class Property : public NotifyChangeSignalable<T>{

 public:
  typedef T value_type;

  // Properties for built-in types are automatically
  // initialized to 0. See template spezialisations
  // at the bottom of this file
  Property()
    : connection_(nullptr)
    , connection_id_(-1) {
#ifdef DEBUG_MY_CODE
	  Serial.println("Property CTOR");
#endif

  }

  Property(T const& val)
    : value_(val)
    , connection_(nullptr)
    , connection_id_(-1) {
#ifdef DEBUG_MY_CODE
	  Serial.println("Property CTOR");
#endif
  }

  Property(T&& val)
    : value_(std::move(val))
    , connection_(nullptr)
    , connection_id_(-1) {
#ifdef DEBUG_MY_CODE
	  Serial.println("Property CTOR");
#endif
  }

  Property(Property<T> const& to_copy)
    : value_(to_copy.value_)
    , connection_(nullptr)
    , connection_id_(-1) {
#ifdef DEBUG_MY_CODE
	  Serial.println("Property CTOR");
#endif
  }

  Property(Property<T>&& to_copy)
    : value_(std::move(to_copy.value_))
    , connection_(nullptr)
    , connection_id_(-1) {
#ifdef DEBUG_MY_CODE
	  Serial.println("Property CTOR");
#endif
  }

  virtual ~Property(){
#ifdef DEBUG_MY_CODE
	  Serial.println("Property DTOR");
#endif
  }

  // sets the Property to a new value. before_change() and
  // on_change() will be emitted.
  virtual void set(T const& value) {
	  if (value != value_) {
		  NotifyChangeSignalable<T>::before_change().emit(value);
		  value_ = value;
		  NotifyChangeSignalable<T>::on_change().emit(value_);
	  }
  }

  // sets the Property to a new value. before_change() and
  // on_change() will not be emitted
  void set_with_no_emit(T const& value) {
    value_ = value;
  }

	// emits before_change() and on_change() even if the value
	// did not change
	void touch() {
		NotifyChangeSignalable<T>::before_change().emit(value_);
		NotifyChangeSignalable<T>::on_change().emit(value_);
	}

  // returns the internal value
  virtual T const& get() const { return value_; }

 // T& getBackdoor() const { return value_; }//TODO: rethink how i use properties. this breaks them. maybe like what i used to from c# - notifies every time a primitive change (or the pointer, but not if the value inside the object changes )

  // connects two Properties to each other. If the source's
  // value is changed, this' value will be changed as well
  virtual void connect_from(Property<T> const& source) {
    disconnect();
    connection_ = &source;
    connection_id_ = source.on_change().connect([this](T const& value){
      set(value);
      return true;
    });
    set(source.get());
  }

  // if this Property is connected from another property,
  // it will e disconnected
  virtual void disconnect() {
    if (connection_) {
      connection_->on_change().disconnect(connection_id_);
      connection_id_ = -1;
      connection_ = nullptr;
    }
  }

  // assigns the value of another Property
  virtual Property<T>& operator=(Property<T> const& rhs) {
    set(rhs.value_);
    return *this;
  }

  // assigns a new value to this Property
  virtual Property<T>& operator=(T const& rhs) {
    set(rhs);
    return *this;
  }

  // compares the values of two Properties
  bool operator==(Property<T> const& rhs) const {
    return Property<T>::get() == rhs.get();
  }
  bool operator!=(Property<T> const& rhs) const {
    return Property<T>::get() != rhs.get();
  }

  // compares the values of the Property to another value
  bool operator==(T const& rhs) const { return Property<T>::get() == rhs; }
  bool operator!=(T const& rhs) const { return Property<T>::get() != rhs; }

  // returns the value of this Property
  T const& operator()() const { return Property<T>::get(); }

  operator T() const { return Property<T>::get(); }

 private:
  T value_;
  Property<T> const* connection_;
  int connection_id_;

};

// specialization for built-in default contructors
template<> inline Property<double>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0.0) {}

template<> inline Property<float>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0.f) {}

template<> inline Property<short>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> inline Property<int>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> inline Property<char>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> inline Property<unsigned>::Property()
  : connection_(nullptr), connection_id_(-1), value_(0) {}

template<> inline Property<bool>::Property()
  : connection_(nullptr), connection_id_(-1), value_(false) {}

// stream operators
/*template<typename T>
std::ostream& operator<<(std::ostream& out_stream, Property<T> const& val) {
  out_stream << val.get();
  return out_stream;
}

template<typename T>
std::istream& operator>>(std::istream& in_stream, Property<T>& val) {
  T tmp;
  in_stream >> tmp;
  val.set(tmp);
  return in_stream;
}*/

}
#endif /* PROPERTY_HPP */
