#pragma once

#include <ft_edcc_base.h>

class ft_get_top_k : public ft_edcc_base {
public:
    map<size_t, EDCC::MatchResult> topKResult;

    void CheckTopKMatchScore(size_t expectK);
};