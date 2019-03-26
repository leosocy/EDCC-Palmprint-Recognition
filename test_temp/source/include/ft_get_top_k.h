#pragma once

#include <ft_edcc_base.h>

class ft_get_top_k : public ft_edcc_base {
public:
    void SetUp();
    void TearDown();

    void ExcuteInterface();

    void SetTopKNum(size_t kNum) {
        this->topKNum = kNum;
    }
    #define FEATURE true
    #define GROUP false
    void SetMode(bool bValue) {
        this->mode = bValue;
    }

    void CheckTopKMatchScore(size_t expectK);

private:
    map<size_t, MatchResult> topKResult;
    size_t topKNum;
    bool mode;
};