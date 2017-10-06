/*************************************************************************
	> File Name: EDCC.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/07/30 14:32:50
 ************************************************************************/
#ifndef __EDCC_H__
#define __EDCC_H__

#define _IN
#define _OUT

namespace EDCC {
    class Adapter {
        public:
            //int train(_IN const char *);
            int parse_cmd(int argc, const char **argv);
    };
}


#endif
