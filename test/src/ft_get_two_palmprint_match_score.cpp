#include <ft_get_two_palmprint_match_score.h>

TEST_F(ft_get_two_palmprint_match_score, Given_Some_Param_NULL_When_Get_Score_Then_EDCC_NULL_POINTER_ERROR)
{
    double matchScore = 0.0;
    int ret = EDCC::GetTwoPalmprintMatchScore(NULL,
                                              ID1_FIRST_PALMPRINT,
                                              CORRECT_CONFIG_PATH,
                                              matchScore);
    EXPECT_EQ(ret, EDCC_NULL_POINTER_ERROR);
}