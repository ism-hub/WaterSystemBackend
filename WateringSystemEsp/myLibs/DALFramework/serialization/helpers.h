/*
 * helpers.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#ifndef DALMODULE_SERIALIZATION_HELPERS_H_
#define DALMODULE_SERIALIZATION_HELPERS_H_


#include <vector>
#include <WString.h>

namespace mycereal {

enum class sfinae {};

template <typename T>
struct has_load_method
{
    struct dummy { /* something */ };

    template <typename C, typename P>
    static auto test(P * p) -> decltype(std::declval<C>().load(*p, *p), std::true_type());

    template <typename, typename>
    static std::false_type test(...);

    typedef decltype(test<T, dummy>(nullptr)) type;
    static const bool value = std::is_same<std::true_type, decltype(test<T, dummy>(nullptr))>::value;
};
//--------------------------------------------

//-------------------------------------------- detail output archive
template <typename T>
struct has_save_method
{
    struct dummy { /* something */ };

    template <typename C, typename P>
    static auto test(P * p) -> decltype(std::declval<C>().save(*p, *p), std::true_type());

    template <typename, typename>
    static std::false_type test(...);

    typedef decltype(test<T, dummy>(nullptr)) type;
    static const bool value = std::is_same<std::true_type, decltype(test<T, dummy>(nullptr))>::value;
};
//--------------------------------------------

//--------------------------------------------

//--------------------------------------------




} /* namespace mycereal */

#endif /* DALMODULE_SERIALIZATION_HELPERS_H_ */
