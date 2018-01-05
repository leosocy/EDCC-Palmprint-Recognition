# API Description

## `GetEDCCCoding`

### Application

Obtain a palmprint EDCC feature code, you can get the data stored in the database to form a palmprint library.

### Parameters

```C++
int GetEDCCCoding(_IN const char *palmprintImagePath,
                  _IN const char *configFileName,
                  _INOUT unsigned char *pCodingBuf,
                  _IN size_t bufMaxLen,
                  _OUT size_t &bufLen);
```

- **palmprintImagePath**: A palmprint image of the path.
- **configFileName**: File path of EDCC algorithm parameter configuration.
- **pCodingBuf**: The address of the encoding feature byte stream buffer pointer.
- **bufMaxLen**: The maximum storage length of the buffer is recommended to be equal to the size of the image configured in config, e.g. ImageSize = (100, 100) => bufMaxLen = 10000.
- **bufLen**: The actual length of the EDCC feature encoding byte stream, bufLen < bufMaxLen.

### Usage

e.g. `config.json` configure imageSizeW = 100, imageSizeH = 100, then bufMaxLen can be set to about 100 * 100.

```C++
size_t bufMaxLen = 1024 * 10;
unsigned char* pCodingBuf = (unsigned char *)malloc(sizeof(unsigned char) * bufMaxLen);
size_t bufLen = 0;
int ret = GetEDCCCoding("../test/palmprint_database/001/1_01_s.bmp",
                        "./config.json",
                        pCodingBuf,
                        bufMaxLen,
                        bufLen);
```

## `GetTwoPalmprintCodingMatchScore`

### Application

Obtained two palmprint code (obtained by GetEDCCCoding) matching score (similarity), the score closer to 1 indicates that the two palmprint similarity is higher.

### Parameters

```C++
int GetTwoPalmprintCodingMatchScore(_IN const unsigned char *firstPalmprintCoding,
                                    _IN const unsigned char *secondPalmprintCoding,
                                    _OUT double &score);
```

- **firstPalmprintCoding**: A palmprint image encoding.
- **secondPalmprintCoding**: Another palmprint image encoding.
- **score**: Match score.

### Usage

```C++
size_t bufMaxLen = 1024 * 10;
unsigned char* pCodingBuf1 = (unsigned char *)malloc(sizeof(unsigned char) * bufMaxLen);
unsigned char* pCodingBuf2 = (unsigned char *)malloc(sizeof(unsigned char) * bufMaxLen);
size_t bufLen1 = 0;
size_t bufLen2 = 0;
int ret = GetEDCCCoding("../test/palmprint_database/001/1_01_s.bmp",
                        "./config.json",
                        pCodingBuf1,
                        bufMaxLen,
                        bufLen1);
ret = GetEDCCCoding("../test/palmprint_database/001/1_01_s.bmp",
                   "./config.json",
                   pCodingBuf2,
                   bufMaxLen,
                   bufLen2);
double score = 0.0;
ret = GetTwoPalmprintCodingMatchScore(pCodingBuf1,
                                      pCodingBuf2,
                                      score);
```

`Tips`: You can also read feature codes from the palmprint database.

## `GetTwoPalmprintMatchScore`

### Application

Obtained two palmprint image matching score (similarity), the score closer to 1 indicates that the two palmprint similarity is higher.

### Parameters

```C++
int GetTwoPalmprintMatchScore(_IN const char *firstPalmprintImagePath,
                              _IN const char *secondPalmprintImagePath,
                              _IN const char *configFileName,
                              _INOUT double &score);
```

- **firstPalmprintImagePath**: A palmprint image of the path.
- **secondPalmprintImagePath**: Another palmprint image of the path.
- **configFileName**: File path of EDCC algorithm parameter configuration.
- **score**: Match score.

### Usage

```C++
double matchScore = 0.0
int ret = GetTwoPalmprintMatchScore("../test/palmprint_database/001/1_01_s.bmp",
                                    "../test/palmprint_database/002/1_01_s.bmp",
                                    "./config.json",
                                    matchScore);
```

## `GetTrainingSetFeatures`

### Application

Obtain the features of all palmprint images in a training set and generate the palmprint database corresponding to each identity.

### Parameters

```C++
int GetTrainingSetFeatures(_IN const char *trainingSetPalmprintGroupFileName,
                           _IN const char *configFileName,
                           _IN const char *featuresOutputFileName,
                           _IN bool isIncremental = false);
```

- **trainingSetPalmprintGroupFileName**: Palmprint training set file path, stored identity and the corresponding palmprint image path information. If a palmprint image path does not exist or there is duplication, the palmprint will be ignored.
- **configFileName**: File path of EDCC algorithm parameter configuration.
- **featuresOutputFileName**: Palmprint features storage path.
- **isIncremental**: Whether to incrementally extract features.
  - True: `configFileName` parameter will be ignored. Read the features stored in featuresOutputFileName and the corresponding algorithm parameters, then extract the palmprint feature in `configFileName` and add it to `featuresOutputFileName`.
  - False: Unconditionally override the data of featuresOutputFileName.

View the template above the input file[Click](https://github.com/Leosocy/EDCC/tree/master/APIInputExample)

### Usage

Non-incremental runs, `isIncremental = false`, the EDCC algorithm computes the palmprint features in `trainingSet.json` based on the configuration in `config.json` and stores them in `trainingSetFeatures.json`.

```C++
int ret = GetTrainingSetFeatures("./trainingSet.json,
                                 "./config.json",
                                 "./trainingSetFeatures.json",
                                 false);
```

Incremental run, `isIncremental = true`, the EDCC algorithm loads the existing feature data and configuration parameters in `trainingSetFeatures.json` and the configuration in `config.json` will be ignored and in computing `trainingSet.json` after the palmprint feature, add the new palmprint feature to `trainingSetFeatures.json`.

```C++
int ret = GetTrainingSetFeatures("./trainingSet.json",
                                 "./config.json",
                                 "./trainingSetFeatures.json",
                                 true);
```

## `GetTopKMatchScore`

### Application

Obtain a Top-K-high matching score and a corresponding identity of a palmprint image with a set of palmprint training sets (or palmprint database).

### Parameters

```C++
int GetTopKMatchScore(_IN const char *onePalmprintImagePath,
                      _IN const char *trainingSetFeaturesOrPalmprintGroupFileName,
                      _IN const char *configFileName,
                      _IN bool isFeatures,
                      _IN size_t K,
                      _INOUT std::map<size_t, MatchResult> &topKResult);
```

- **onePalmprintImagePath**: A palmprint image of the path.
- **trainingSetFeaturesOrPalmprintGroupFileName**: File path of palm print image training set or palmprint database.
- **configFileName**: File path of EDCC algorithm parameter configuration.
- **isFeatures**: Participation `trainingSetFeaturesOrPalmprintGroupFileName` is a palmprint feature library.
- **K**: Get the score of the top K high match.
- **topKResult**: Matches the results, sorted according to the matching score from high to low in map, eg. TopKResult.at (1) is the first matching result of matching score.

Parameters in the MatchResult structure:

- `identity`: Palmprint identity.
- `imagePath`: Palmprint image path.
- `score`: Match score.
- `rank`: The result of this match ranks among the K results.

### Usage

Match the palmprint in the palmprint database.

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

Match palmprint in the training set.

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

## EDCC return value description

- **EDCC_SUCCESS**: EDCC API processing is successful.
- **EDCC_NULL_POINTER_ERROR**: API into parameters exist null pointer.
- **EDCC_LOAD_CONFIG_FAIL**: Load EDCC algorithm configuration file failed.

    Possible Causes:

        1. Path does not exist.
        2. The algorithm parameter name in the configuration file is invalid or missing.
        3. The configuration value of the algorithm in the configuration file is invalid.
        4. The value of the parameter in the signature library file is illegal.
        5. The json file format problem.

    `Tips`: If the API returns this failure code, you can refer to this [config.json](https://github.com/Leosocy/EDCC/blob/master/APIInputExample/config.json)

- **EDCC_LOAD_TAINING_SET_FAIL**: Fail to load palm print training set file.

    Possible Causes:

        1. Path does not exist.
        2. The json file format problem.

    `Tips`: If the API returns this failure code, you can refer to this [Palmprint Set](https://github.com/Leosocy/EDCC/blob/master/APIInputExample/trainingSet.json)

- **EDCC_LOAD_FEATURES_FAIL**: Load palmprint feature library file failed.

    Possible Causes:

        1. This error usually does not occur unless you manually modified the palmprint library file.

    `Tips`: Do not modify the palmprint library file.

- **EDCC_SAVE_FEATURES_FAIL**: Save palmprint features database failed.

    Possible Causes:

        1. The features file can not be saved.

    `Tips`: Please ensure that the current user has permission to operate the corresponding path.

- **EDCC_LOAD_PALMPRINT_IMAGE_FAIL**: Loaded palmprint image failed.

    Possible Causes:

        1. Palmprint image path does not exist.
        2. This file path is not a format that OpenCV can parse.

- **EDCC_CODING_BUFF_LEN_NOT_ENOUGH**: The byte stream is not long enough.

    Possible Causes:

        1. The buffer size passed in when calling GetEDCCCoding is not enough.

    `Tips`: BufMaxLen can be configured by entering the palmprint image size to calculate.

- **EDCC_CODING_INVALID**: The EDCC coding has been modified.

    Possible Causes:

        1. The edcc coding has been modified and does not meet the standard.

    `Tips`: Do not changed EDCC coding.

- **EDCC_CODINGS_DIFF_CONFIG**: Two EDCC coding use different config.

    Possible Causes:

        1. When you use GetTwoPalmprintCodingMatchScore, please make sure two EDCC coding use same config.