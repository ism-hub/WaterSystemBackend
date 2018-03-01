/*
 * StringUtils.h
 *
 *  Created on: 16 áôáø× 2018
 *      Author: IsM
 */

#ifndef LIBRARIES_STRINGUTILS_STRINGUTILS_H_
#define LIBRARIES_STRINGUTILS_STRINGUTILS_H_

#include <vector>
#include <WString.h>

namespace stru  {


/*std::vector<String> split(const char *str, char c = '/')
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
}*/

/*std::vector<String> split2(const char *str, char c = '/')
{
	std::vector<String> result;
	String string(str);
	const char* start = str;
    do
    {
        const char *begin = str;
        while(*str != c && *str)
            str++;
        result.push_back(string.substring(begin - start, str - start));
    } while (0 != *str++);

    return result;
}*/

/*std::vector<String> ssplit2(const char *str, char c = '/')
{
	std::vector<String> result;
	String string(str);
	const char* start = str;
    do
    {
        const char *begin = str;
        while(*str != c && *str)
            str++;
        result.push_back(string.substring(begin - start, str - start));
    } while (0 != *str++);

    return result;
}*/

}

#endif /* LIBRARIES_STRINGUTILS_STRINGUTILS_H_ */
