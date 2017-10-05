/*************************************************************************
    > File Name: Pub.h
    > Author: Leosocy
    > Mail: 513887568@qq.com
    > Created Time: 2017/10/1 23:05:54
************************************************************************/
#ifndef __PUB_H__
#define __PUB_H__

#include <string>
using namespace std;

namespace EDCC {
    static const char hexArray[16] = {'0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'A', 'B',
        'C', 'D', 'E', 'F'};

    #define CHECK_POINTER_NULL_RETURN(pointer, ret) do { \
        if((pointer) == NULL) { \
            return (ret); \
        } \
    } while(0)

    #define CHECK_POINTER_NULL_RETURN_VOID(pointer) do { \
        if((pointer) == NULL) { \
            return ; \
        } \
    } while(0)

    string toUpper(const char *src);
    string toLower(const char *src);

}

#endif // !__PUB_H__
