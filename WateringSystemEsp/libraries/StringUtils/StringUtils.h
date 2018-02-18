/*
 * StringUtils.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#ifndef LIBRARIES_STRINGUTILS_STRINGUTILS_H_
#define LIBRARIES_STRINGUTILS_STRINGUTILS_H_

namespace stru  {

#include <vector>
#include <WString.h>

std::vector<String> split(const char *str, char c = '/')
{
	std::vector<String> result;
    do
    {
        const char *begin = str;
        while(*str != c && *str)
            str++;

        result.push_back(String(begin, str));
    } while (0 != *str++);

    return result;
}

}

#endif /* LIBRARIES_STRINGUTILS_STRINGUTILS_H_ */
