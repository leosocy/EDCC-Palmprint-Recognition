#include <ft_edcc_base.h>

void ft_edcc_base::CheckFeaturesConfigEqualConfigFile(const char *featuresFileName,
                                                      const char *configFileName)
{
    Json::Value featuresRoot, configRoot;
    Json::Reader reader;
    ifstream featuresIn, configIn;
    featuresIn.open(featuresFileName);
    configIn.open(configFileName);
    if(!reader.parse(featuresIn, featuresRoot)
       || !reader.parse(configIn, configRoot)) {
        EXPECT_STREQ("config format", "error");
        return;
    }

    if(featuresRoot["imageSize"].isNull()
       || featuresRoot["laplaceKernelSize"].isNull()
       || featuresRoot["gaborKernelSize"].isNull()
       || featuresRoot["gaborDirections"].isNull()
       || configRoot["imageSize"].isNull()
       || configRoot["laplaceKernelSize"].isNull()
       || configRoot["gaborKernelSize"].isNull()
       || configRoot["gaborDirections"].isNull()) {
        EXPECT_STREQ("config format", "error");
        return;
    }

    EXPECT_EQ(featuresRoot["imageSize"].asInt(), configRoot["imageSize"]["default"].asInt());
    EXPECT_EQ(featuresRoot["laplaceKernelSize"].asInt(), configRoot["laplaceKernelSize"]["default"].asInt());
    EXPECT_EQ(featuresRoot["gaborKernelSize"].asInt(), configRoot["gaborKernelSize"]["default"].asInt());
    EXPECT_EQ(featuresRoot["gaborDirections"].asInt(), configRoot["gaborDirections"]["default"].asInt());
}

void ft_edcc_base::CheckOneIdentityImageCountInFeatures(const char *featuresFileName,
                                                        const char *identity,
                                                        int expectedCount)
{
    Json::Value featuresRoot;
    Json::Reader reader;
    ifstream featuresIn, configIn;
    featuresIn.open(featuresFileName);
    if(!reader.parse(featuresIn, featuresRoot)) {
        EXPECT_STREQ("config format", "error");
        return;
    }
    if(featuresRoot[identity].isNull()
       || !featuresRoot[identity].isObject())
    {
        EXPECT_STREQ("Identity", "not exists");
        return;
    }
    
    Json::Value::Members members = featuresRoot[identity].getMemberNames();
    int actualCount = members.size();
    EXPECT_EQ(actualCount, expectedCount);
}