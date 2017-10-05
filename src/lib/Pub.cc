/*************************************************************************
> File Name: Pub.cc
> Author: Leosocy
> Mail: 513887568@qq.com
> Created Time: 2017/10/5 11:12:26
************************************************************************/
#include <Pub.h>
using namespace EDCC;

string EDCC::toUpper(const char *src)
{
    string str;
    CHECK_POINTER_NULL_RETURN(src, str);

    str = src;
    for(size_t i = 0; i < str.length(); ++i) {
        str[i] = toupper(str[i]);
    }

    return str;
}

string EDCC::toLower(const char *src)
{
    string str;
    CHECK_POINTER_NULL_RETURN(src, str);

    str = src;
    for(size_t i = 0; i < str.length(); ++i) {
        str[i] = tolower(str[i]);
    }

    return str;
}