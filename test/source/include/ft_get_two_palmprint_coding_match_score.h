#pragma once
#include <ft_edcc_base.h>

class ft_get_two_palmprint_coding_match_score : public ft_edcc_base {
public:
    void SetUp();
    void TearDown();

    void ExcuteInterface();

    void GenFirstPalmprintCodingBuff(const char *firstPalmprintImagePath);
    void GenSecondPalmprintCodingBuff(const char *secondPalmprintImagePath);
    void GenMatchScoreBaseline();

    void SetFirstPalmprintCoding(unsigned char *codingBuff);
    void SetSecondPalmprintCoding(unsigned char *codingBuff);
    void ChangePalmprintCoding();

    void CheckMatchScoreEQ(double expectScore);
    void CheckMatchScoreNE(double expectScore);
    void CheckMatchScoreEQToPalmprint();
    void CheckMatchScoreNEToPalmprint();
    void CheckMatchScoreEQWhenCodingModeAndMatchingModeChanged();
private:
    void GenCodingBuff(const char *imagePath, char **dstImagePath, unsigned char **codingBuff);
    unsigned char *firstCodingBuff;
    char *firstImagePath;
    unsigned char *secondCodingBuff;
    char *secondImagePath;
    double matchScoreWithPalmprintCoding;
    double matchScoreWithPalmprint;
    std::vector<double> matchScoreWithPalmprintCodingVec;
    std::vector<double> matchScoreWithPalmprintVec;
    size_t bufMaxLen;
    size_t bufLen;
};