#pragma once

#include <ft_edcc_base.h>

class ft_get_two_palmprint_match_score : public ft_edcc_base {
public:
    void SetUp();
    void TearDown();

    void ExcuteInterface();

    void SetFirstPalmprintImagePath(const char *path);
    void SetSecondPalmprintImagePath(const char *path);

    void CheckMatchScoreEQ(double expectScore);
    void CheckMatchScoreNE(double expectScore);
private:
    char *firstImagePath;
    char *secondImagePath;
    double matchScore;
};