/*
 * helpers2.h
 *
 *  Created on: 31 αιεμι 2017
 *      Author: IsM
 */

#ifndef SERIALIZATION_HELPERS2_H_
#define SERIALIZATION_HELPERS2_H_



#include <type_traits>
#include <utility>
#include <WString.h>

namespace cereal2 {

namespace detail2 {
struct NameValuePairCore {};//!< Traits struct for NVPs

// base classes for type checking
    /* The rtti virtual function only exists to enable an archive to
       be used in a polymorphic fashion, if necessary.  See the
       archive adapters for an example of this */
class OutputArchiveBase {

};

class InputArchiveBase
    {
      public:
        InputArchiveBase() = default;
        InputArchiveBase( InputArchiveBase && ) noexcept {}
        InputArchiveBase & operator=( InputArchiveBase && ) noexcept { return *this; }
        virtual ~InputArchiveBase() noexcept = default;

      private:
        virtual void rtti() {}
    };



}

template <class T>
 class NameValuePair : detail2::NameValuePairCore
 {
   private:
     // If we get passed an array, keep the type as is, otherwise store
     // a reference if we were passed an l value reference, else copy the value
     using Type = typename std::conditional<std::is_array<typename std::remove_reference<T>::type>::value,
                                            typename std::remove_cv<T>::type,
                                            typename std::conditional<std::is_lvalue_reference<T>::value,
                                                                      T,
                                                                      typename std::decay<T>::type>::type>::type;

     // prevent nested nvps
     static_assert( !std::is_base_of<detail2::NameValuePairCore, T>::value,
                    "Cannot pair a name to a NameValuePair" );

     NameValuePair & operator=( NameValuePair const & ) = delete;

   public:
     //! Constructs a new NameValuePair
     /*! @param n The name of the pair
         @param v The value to pair.  Ideally this should be an l-value reference so that
                  the value can be both loaded and saved to.  If you pass an r-value reference,
                  the NameValuePair will store a copy of it instead of a reference.  Thus you should
                  only pass r-values in cases where this makes sense, such as the result of some
                  size() call.
         @internal */
     NameValuePair(String n, T && v ) : name(n), value(std::forward<T>(v)) {}

     String name;
     Type value;
 };

} /* namespace cereal2 */

#endif /* SERIALIZATION_HELPERS2_H_ */
