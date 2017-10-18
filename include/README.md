# API说明

## `GetTrainingSetFeatures`

### 功能

计算一个训练集中所有掌纹图像的特征，并生成每一个身份对应的掌纹特征库。

### 入参说明

```C++
int GetTrainingSetFeatures(_IN const char *trainingSetPalmprintGroupFileName,
                           _IN const char *configFileName,
                           _IN const char *featuresOutputFileName,
                           _IN bool isIncremental = false);
```

- **trainingSetPalmprintGroupFileName**: 掌纹训练集文件的路径，存储了身份与对应的掌纹图像路径信息。若某个掌纹图像路径不存在或者存在重复，该掌纹将会被忽略。
- **configFileName**: EDCC算法参数配置的文件路径。
- **featuresOutputFileName**: 掌纹特征的存储路径。
- **isIncremental**: 是否增量提取特征。
  - 是：`configFileName`参数将被忽略。读取`featuresOutputFileName`中存储的特征以及对应的算法参数，然后提取`configFileName`中的掌纹特征并加入到`featuresOutputFileName`中。
  - 否：无条件覆盖`featuresOutputFileName`的数据。

查看以上输入文件的模板[请点击](https://github.com/Leosocy/EDCC/tree/master/APIInputExample)

### 用法

非增量运行，`isIncremental = false`，
EDCC算法根据`config.json`中的配置计算`trainingSet.json`中的掌纹特征，
并存储到`trainingSetFeatures.json`中。

```C++
int ret = GetTrainingSetFeatures("./trainingSet.json,
                                 "./config.json",
                                 "./trainingSetFeatures.json",
                                 false);
```

增量运行，`isIncremental = true`，
EDCC算法加载`trainingSetFeatures.json`中已有的特征数据和配置参数，而`config.json`中的配置将被忽略，
在计算`trainingSet.json`中的掌纹特征后，将新增的掌纹特征加入到`trainingSetFeatures.json`中。

```C++
int ret = GetTrainingSetFeatures("./trainingSet.json",
                                 "./config.json",
                                 "./trainingSetFeatures.json",
                                 true);
```

## `GetTwoPalmprintMatchScore`

### 功能

获得两幅掌纹图像的匹配得分(相似度)，得分越接近1表明两个掌纹相似度越高。

### 入参说明

```C++
int GetTwoPalmprintMatchScore(_IN const char *firstPalmprintImagePath,
                              _IN const char *secondPalmprintImagePath,
                              _IN const char *configFileName,
                              _INOUT double &score);
```

- **firstPalmprintImagePath**: 一幅掌纹图像的路径。
- **secondPalmprintImagePath**: 另一幅掌纹图像的路径。
- **configFileName**: EDCC算法参数配置的文件路径。
- **score**: 获得的匹配得分。

### 用法

```C++
double matchScore = 0.0
int ret = EDCC::GetTwoPalmprintMatchScore("../test/palmprint_database/001/1_01_s.bmp",
                                          "../test/palmprint_database/002/1_01_s.bmp",
                                          "./config.json",
                                          matchScore);
```

## `GetTopKMatchScore`

### 功能

获得一幅掌纹图像与一组掌纹训练集(或者掌纹特征库)的前K高匹配得分及对应的身份。

### 入参说明

```C++
int GetTopKMatchScore(_IN const char *onePalmprintImagePath,
                      _IN const char *trainingSetFeaturesOrPalmprintGroupFileName,
                      _IN const char *configFileName,
                      _IN bool isFeatures,
                      _IN size_t K,
                      _INOUT std::map<size_t, MatchResult> &topKResult);
```

- **onePalmprintImagePath**: 一幅掌纹图像的路径。
- **trainingSetFeaturesOrPalmprintGroupFileName**: 掌纹图像训练集或者掌纹特征库的文件路径。
- **configFileName**: EDCC算法参数配置的文件路径。
- **isFeatures**: 入参`trainingSetFeaturesOrPalmprintGroupFileName`是否为掌纹特征库。
- **K**: 获取前K高的匹配的分。
- **topKResult**: 匹配的结果，map中按匹配得分从高至低排序,e.g. topKResult.at(1)为匹配得分第1的匹配结果。

  MatchResult结构体中的参数说明:
  - `identity`: 掌纹身份
  - `imagePath`: 掌纹图像路径
  - `score`: 匹配得分
  - `rank`: 此项匹配结果在K个结果中的得分排名

### 用法

匹配掌纹特征库中的掌纹

```C++
size_t K = 10;
std::map<size_t, MatchResult> topKResult;
int ret = GetTopKMatchScore("../test/palmprint_database/001/1_01_s.bmp",
                            "./trainingSetFeatures.json",
                            "./config.json",
                            true,
                            K,
                            topKResult);
```

匹配掌纹训练集中的掌纹

```C++
size_t K = 10;
std::map<size_t, MatchResult> topKResult;
int ret = GetTopKMatchScore("../test/palmprint_database/001/1_01_s.bmp",
                            "./trainingSet.json",
                            "./config.json",
                            false,
                            K,
                            topKResult);
```

## 返回值说明

- **EDCC_SUCCESS**: EDCC API处理成功。
- **EDCC_NULL_POINTER_ERROR**: API入参存在空指针。
- **EDCC_LOAD_CONFIG_FAIL**: 加载EDCC算法配置文件出错。

  可能原因：
  1. 路径不存在
  1. 配置文件中的算法参数名非法或缺失
  1. 配置文件中的算法配置值非法
  1. 特征库文件中的参数值非法
  1. json文件格式问题

  Tips: 如果API返回此失败码，可以参考此[算法配置文件](https://github.com/Leosocy/EDCC/blob/master/APIInputExample/config.json)
- **EDCC_LOAD_TAINING_SET_FAIL**: 加载掌纹训练集文件出错。

  可能原因：
  1. 路径不存在
  1. json文件格式问题

  Tips: 如果API返回此失败码，可以参考此[掌纹训练集文件](https://github.com/Leosocy/EDCC/blob/master/APIInputExample/trainingSet.json)
- **EDCC_LOAD_FEATURES_FAIL**: 加载掌纹特征库文件出错。

  可能原因:
  1. 通常不会出现此错误，除非您手动修改了掌纹特征库文件

  Tips: 请勿修改掌纹特征库文件
- **EDCC_SAVE_FEATURES_FAIL**: 保存掌纹特征库出错。

  可能原因:
  1. 特征库的保存路径文件无法创建

  Tips: 请确保当前用户有权限操作对应的路径

- **EDCC_LOAD_PALMPRINT_IMAGE_FAIL**: 加载掌纹图像失败。

  可能原因:
  1. 掌纹图像的路径不存在
  1. 该文件路径不是OpenCV可以解析的格式