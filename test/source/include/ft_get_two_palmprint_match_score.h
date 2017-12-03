#pragma once

#include <ft_edcc_base.h>

class ft_get_two_palmprint_match_score : public ft_edcc_base {
public:
    virtual void SetUp();
    virtual void TearDown();

    virtual void ExcuteInterface();

    virtual void SetFirstPalmprintImagePath(const char *path);
    virtual void SetSecondPalmprintImagePath(const char *path);

    virtual void CheckMatchScoreEQ(double expectScore);
    virtual void CheckMatchScoreNE(double expectScore);
private:
    char *firstImagePath;
    char *secondImagePath;
    double matchScore;
};