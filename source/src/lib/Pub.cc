/*************************************************************************
    > File Name: Pub.cc
    > Author: Leosocy
    > Mail: 513887568@qq.com
    > Created Time: 2017/10/5 11:12:26
************************************************************************/
#include <Pub.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
using namespace EDCC;

std::string EDCC::toUpper(_IN const char *src)
{
    std::string str;
    CHECK_POINTER_NULL_RETURN(src, str);

    str = src;
    for(size_t i = 0; i < str.length(); ++i) {
        str[i] = toupper(str[i]);
    }

    return str;
}

void EDCC::EDCC_Log(_IN const char *format, ...)
{
    char log[1024] = {0};
    va_list args;
    va_start(args, format);
    std::string formatStr = format;
    formatStr = "[EDCC Debug]\t" + formatStr + "\n";
    vsnprintf(log, sizeof(log), formatStr.c_str(), args);
    va_end(args);

    #ifdef _WINDOWS
    printf("%s", log);

    #else // DEBUG
    FILE *outFile;
    outFile = fopen("./edcc.log", "a+");
    CHECK_POINTER_NULL_RETURN_VOID(outFile);
    fwrite(log, strlen(log), 1, outFile);
    fclose(outFile);
    #endif
}
