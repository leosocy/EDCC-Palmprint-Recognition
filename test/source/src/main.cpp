/*************************************************************************
> File Name: main.cpp
> Author: Leosocy
> Mail: 513887568@qq.com
> Created Time: 2017/10/5 22:51:38
************************************************************************/
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::GTEST_FLAG(filter) = "*Given_Features_And_IncrementalGroup_Has_Cover_When_Train_With_Incremental_Then_EDCC_SUCCESS_And_Config_Count_Correct_And_Features_Cover";
    return RUN_ALL_TESTS();
}
