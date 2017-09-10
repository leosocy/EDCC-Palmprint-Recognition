/*************************************************************************
	> File Name: Match.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/09/10 20:50:15
 ************************************************************************/

#ifndef __CHECK_H__
#define __CHECK_H_

namespace EDCC
{
    class Check {
        public:
            bool checkConfigValid(const map<string, int> &configMap);
            bool checkPalmprintGroupValid(const vector<PalmprintCode> &data);
            bool checkPalmprintFeatureData(const vector<PalmprintCode> &data);
        private:
    };
}

#endif