#pragma once
#include <ft_edcc_base.h>

class ft_get_two_palmprint_coding_match_score : public ft_edcc_base {
public:
    void SetUp();
    void TearDown();

    void ExcuteInterface();

    void GenFirstPalmprintCodingBuff(const char *firstPalmprintImagePath);
    void GenSecondPalmprintCodingBuff(const char *secondPalmprintImagePath);

    void SetFirstPalmprintCoding(unsigned char *codingBuff);
    void SetSecondPalmprintCoding(unsigned char *codingBuff);
    void ChangePalmprintCodingMagicKey();

    void CheckMatchScoreEQ(double expectScore);
    void CheckMatchScoreNE(double expectScore);
    void CheckMatchScoreEQToPalmprint();
    void CheckMatchScoreNEToPalmprint();
private:
    unsigned char *firstCodingBuff;
    char *firstImagePath;
    unsigned char *secondCodingBuff;
    char *secondImagePath;
    double matchScoreWithPalmprintCoding;
    double matchScoreWithPalmprint;
    size_t bufMaxLen;
    size_t bufLen;
};