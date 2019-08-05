/*
 * traits2.h
 *
 *  Created on: 4 баев„ 2017
 *      Author: IsM
 */

#ifndef SERIALIZATION_TRAITS2_H_
#define SERIALIZATION_TRAITS2_H_

#include <type_traits>
#include <typeindex>

namespace cereal2 {
namespace traits2 {

//! Type traits only struct used to mark an archive as human readable (text based)
 /*! Archives that wish to identify as text based/human readable should inherit from
     this struct */
 struct TextArchive {};
}

namespace detail2 {
//! Return type for SFINAE Enablers
enum class sfinae {};

//! Used as the default value for EnableIf and DisableIf template parameters
/*! @relates EnableIf
  @relates DisableIf */
static const detail2::sfinae sfinae2 = {};

template<typename, typename T>
struct has_save_method {
    static_assert(
        std::integral_constant<T, false>::value,
        "has_save_method have failed : Second template parameter needs to be of function type.");
};

template<typename C, typename Ret, typename... Args>
struct has_save_method<C, Ret(Args...)> {
private:
    template<typename T>
    static constexpr auto check(T*)-> typename std::is_same< decltype( std::declval<T>().save( std::declval<Args>()... ) ), Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
																																	>::type;  // attempt to call it and see if the return type is correct

    template<typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

template<typename, typename T>
struct has_load_method {
    static_assert(
        std::integral_constant<T, false>::value,
        "has_load_method failed : Second template parameter needs to be of function type.");
};

template<typename C, typename Ret, typename... Args>
struct has_load_method<C, Ret(Args...)> {
private:
    template<typename T>
    static constexpr auto check(T*)-> typename std::is_same< decltype( std::declval<T>().load( std::declval<Args>()... ) ), Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
																																	>::type;  // attempt to call it and see if the return type is correct

    template<typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

// ######################################################################
//! Provides a way to enable a function if conditions are met
/*! This is intended to be used in a near identical fashion to std::enable_if
  while being significantly easier to read at the cost of not allowing for as
  complicated of a condition.

  This will compile (allow the function) if every condition evaluates to true.
  at compile time.  This should be used with SFINAE to ensure that at least
  one other candidate function works when one fails due to an EnableIf.

  This should be used as the las template parameter to a function as
  an unnamed parameter with a default value of cereal::traits::sfinae:

  @code{cpp}
  // using by making the last template argument variadic
  template <class T, EnableIf<std::is_same<T, bool>::value> = sfinae>
  void func(T t );
  @endcode

  Note that this performs a logical AND of all conditions, so you will need
  to construct more complicated requirements with this fact in mind.

  @relates DisableIf
  @relates sfinae
  @tparam Conditions The conditions which will be logically ANDed to enable the function. */
//template <bool ... Conditions>
//using EnableIf = typename detail2::EnableIfHelper<Conditions...>::type;

} /* namespace traits2 */
}/* namespace cereal2 */
#endif /* SERIALIZATION_TRAITS2_H_ */
