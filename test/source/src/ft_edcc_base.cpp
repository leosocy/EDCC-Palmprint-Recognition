#include <ft_edcc_base.h>

ft_edcc_base::ft_edcc_base()
{
}

ft_edcc_base::~ft_edcc_base()
{
}

void ft_edcc_base::SetUp()
{
    CopyDir(EXAMPLE_SRC_DIR, EXAMPLE_DST_DIR);

    this->configPath = NULL;
    this->groupPath = NULL;
    this->featurePath = NULL;
    this->imagePath = NULL;
}

void ft_edcc_base::TearDown()
{
   // DeleteDir(EXAMPLE_DST_DIR);

    FREE_CHAR_ARRAY(this->configPath);
    FREE_CHAR_ARRAY(this->groupPath);
    FREE_CHAR_ARRAY(this->featurePath);
    FREE_CHAR_ARRAY(this->imagePath);
}

void ft_edcc_base::CheckInterfaceRet(int expectRet)
{
    EXPECT_EQ(interRet, expectRet);
}

void ft_edcc_base::CheckFeaturesConfigEqualConfigFile(const char *featuresFileName,
                                                      const char *configFileName)
{
    EXPECT_TRUE(featuresFileName != NULL);
    EXPECT_TRUE(configFileName != NULL);
    if(featuresFileName == NULL
       || configFileName == NULL) {
        return;
    }

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

    if(featuresRoot["imageSizeW"].isNull()
       || featuresRoot["imageSizeH"].isNull()
       || featuresRoot["laplaceKernelSize"].isNull()
       || featuresRoot["gaborKernelSize"].isNull()
       || featuresRoot["gaborDirections"].isNull()
       || configRoot["imageSizeW"].isNull()
       || configRoot["imageSizeH"].isNull()
       || configRoot["laplaceKernelSize"].isNull()
       || configRoot["gaborKernelSize"].isNull()
       || configRoot["gaborDirections"].isNull()) {
        EXPECT_STREQ("config format", "error");
        return;
    }

    EXPECT_EQ(featuresRoot["imageSizeW"].asInt(), configRoot["imageSizeW"]["default"].asInt());
    EXPECT_EQ(featuresRoot["imageSizeH"].asInt(), configRoot["imageSizeH"]["default"].asInt());
    EXPECT_EQ(featuresRoot["laplaceKernelSize"].asInt(), configRoot["laplaceKernelSize"]["default"].asInt());
    EXPECT_EQ(featuresRoot["gaborKernelSize"].asInt(), configRoot["gaborKernelSize"]["default"].asInt());
    EXPECT_EQ(featuresRoot["gaborDirections"].asInt(), configRoot["gaborDirections"]["default"].asInt());
}

void ft_edcc_base::CheckOneIdentityImageCountInFeatures(const char *featuresFileName,
                                                        const char *identity,
                                                        size_t expectedCount)
{
    EXPECT_TRUE(featuresFileName != NULL);
    EXPECT_TRUE(identity != NULL);
    if(featuresFileName == NULL
       || identity == NULL) {
        return;
    }

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
    size_t actualCount = members.size();
    EXPECT_EQ(actualCount, expectedCount);
}

void ft_edcc_base::AppendConfigParam(const char *key,
                                     int value)
{
    ParseJsonFile(configPath);
    valueRoot[key] = value;
    ofstream configOut;
    configOut.open(configPath);
    configOut << valueRoot.toStyledString();
    configOut.close();
}

void ft_edcc_base::RemoveConfigParam(const char *paramName)
{
    ParseJsonFile(configPath);
    valueRoot.removeMember(paramName);
    ofstream configOut;
    configOut.open(configPath);
    configOut << valueRoot.toStyledString();
    configOut.close();
}

void ft_edcc_base::ModifyConfigParams(const char *paramName,
                                      int valueToSet)
{
    Json::Value *configValue;
    configValue = GetJsonValueByConfigParamName(configPath, paramName);
    if(configValue == NULL) {
        EXPECT_STREQ(paramName, "not exists"); 
        return;
    }
    *configValue = valueToSet;

    ofstream configOut;
    configOut.open(configPath);
    configOut << valueRoot.toStyledString();
    configOut.close();
}

void ft_edcc_base::CheckConfigParams(const char *paramName,
                                     int expectValue)
{
    Json::Value *configValue;
    configValue = GetJsonValueByConfigParamName(configPath, paramName);
    if(configValue == NULL) {
        EXPECT_STREQ(paramName, "not exists"); 
        return;
    }
    EXPECT_EQ(configValue->asInt(), expectValue);
}

void ft_edcc_base::CheckConfigParamExists(const char *paramName)
{
    Json::Value *configValue;
    configValue = GetJsonValueByConfigParamName(configPath, paramName);
    EXPECT_TRUE(configValue != NULL);
}

void ft_edcc_base::CheckConfigParamNotExists(const char *paramName)
{
    Json::Value *configValue;
    configValue = GetJsonValueByConfigParamName(configPath, paramName);
    EXPECT_TRUE(configValue == NULL);
}

Json::Value* ft_edcc_base::GetJsonValueByConfigParamName(const char *config_path,
                                                         const char *paramName)
{
    ParseJsonFile(config_path);
    if(valueRoot[paramName].isNull())
    {
        return NULL;
    }
    if(valueRoot[paramName].isInt()) {
        return &valueRoot[paramName];
    } else if(valueRoot[paramName].isObject()
              && valueRoot[paramName]["default"].isInt()) {
        return &valueRoot[paramName]["default"];
    } else{
        EXPECT_STREQ("config format", "error");  
        return NULL;
    }
}

void ft_edcc_base::ParseJsonFile(const char *config_path)
{
    valueRoot.clear();
    Json::Reader reader;

    EXPECT_TRUE(config_path != NULL);
    if(config_path == NULL) {
        return;
    }

    ifstream configIn;
    configIn.open(config_path);
    if(!reader.parse(configIn, valueRoot)) {
        EXPECT_STREQ("config format", "error");
        configIn.close();
        return;
    }
    configIn.close();
}

void ft_edcc_base::CopyDir(const char *srcDir, const char *dstDir)
{
    char opr[1024] = {0};

    #ifdef _WINDOWS
    snprintf(opr, sizeof(opr), "xcopy /s /e /y %s %s > ft_log.txt", srcDir, dstDir);  
    #else
    snprintf(opr, sizeof(opr), "cp -rf %s %s > ft_log.txt", srcDir, dstDir);
    #endif

    system(opr);
}
void ft_edcc_base::CopyFile(const char *srcFile, const char *dstFile)
{
    char opr[1024] = {0};

    #ifdef _WINDOWS
    snprintf(opr, sizeof(opr), "echo f | xcopy /y %s %s > ft_log.txt", srcFile, dstFile);
    #else
    snprintf(opr, sizeof(opr), "cp -rf %s %s > ft_log.txt", srcFile, dstFile);
    #endif

    system(opr);
}

void ft_edcc_base::DeleteDir(const char *srcDir)
{
    char opr[1024] = {0};

    #ifdef _WINDOWS
    snprintf(opr, sizeof(opr), "rmdir /s /q %s > ft_log.txt", srcDir);
    #else
    snprintf(opr, sizeof(opr), "rm -rf %s > ft_log.txt", srcDir);
    #endif

    system(opr);
}
void ft_edcc_base::DeleteFile(const char *srcFile)
{
    char opr[1024] = {0};

    #ifdef _WINDOWS
    snprintf(opr, sizeof(opr), "del /f /s /q %s > ft_log.txt", srcFile);
    #else
    snprintf(opr, sizeof(opr), "rm -rf %s > ft_log.txt", srcFile);
    #endif

    system(opr);
}