#pragma once

#include <ft_edcc_base.h>

class ft_get_features_incremental : public ft_edcc_base {
public:
    void SetUp();
    void TearDown();

    void ExcuteInterface();

    void SetIncremental() {
        this->bIncremental = true;
    }

    void GenBaseData();

private:
    bool bIncremental;

};