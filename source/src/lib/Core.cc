/*************************************************************************
    > File Name: Core.cc
    > Author: Leosocy
    > Mail: 513887568@qq.com 
    > Created Time: 2017/07/30 00:11:24
 ************************************************************************/
#include <Core.h>
using namespace EDCC;

//-----------------------------------Palmprint-------------------------------------

Palmprint::Palmprint(_IN const char *identity, _IN const char *imagePath)
{
    if(identity == NULL || imagePath == NULL) {
        EDCC_Log("Identity or imagePath can't be null!\n");
        return ;
    }

    this->identity = identity;
    this->imagePath = imagePath;
}

Palmprint::~Palmprint()
{
}

Palmprint& Palmprint::operator =(_IN const Palmprint &other)
{
    this->identity = other.identity;
    this->imagePath = other.imagePath;
    this->image = other.image.clone();
    return *this;
}

Palmprint::Palmprint(_IN const Palmprint &other)
{
    this->identity = other.identity;
    this->imagePath = other.imagePath;
    this->image = other.image.clone();
}

bool Palmprint::operator==(_IN const Palmprint &p) const
{
    return this->identity == p.identity
        && this->imagePath == p.imagePath;
}

string Palmprint::getIdentity() const
{
    return this->identity;
}

string Palmprint::getImagePath() const
{
    return this->imagePath;
}

cv::Mat* Palmprint::genOriImg()
{
    image = imread(this->imagePath, CV_LOAD_IMAGE_COLOR);
    if(!image.data) {
        EDCC_Log("Read image failed!\n");
        return NULL;
    }
    return &image;
}

cv::Mat* Palmprint::genSpecImg(_IN const cv::Size &imgSize, _IN bool isGray)
{
    Mat *ptOriImg = genOriImg();
    CHECK_POINTER_NULL_RETURN(ptOriImg, NULL);
    resize(*ptOriImg, *ptOriImg, imgSize);
    if(isGray) {
        cvtColor(*ptOriImg, *ptOriImg, CV_BGR2GRAY);
    }
    return ptOriImg;
}

//---------------------------------EDCCoding------------------------------------

EDCCoding::EDCCoding()
{
    ptCoding = NULL;
    magicKey = 0x0622520a;
}

EDCCoding::~EDCCoding()
{
    freeCoding();
}

EDCCoding& EDCCoding::operator =(_IN const EDCCoding &other)
{
    EDCCoding::EDCCoding(other);

    return *this;
}

EDCCoding::EDCCoding(_IN const EDCCoding &other)
{
    this->C = other.C.clone();
    this->Cs = other.Cs.clone();
    memcpy(&this->cfg, &other.cfg, sizeof(EDCC_CFG_T));
    this->magicKey = other.magicKey;
    this->ptCoding = NULL;
    if(other.ptCoding != NULL) {
        this->ptCoding = (EDCC_CODING_T*)malloc(other.ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));
        memcpy(this->ptCoding, other.ptCoding, other.ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));
    }
}

size_t EDCCoding::encrypt(_INOUT unsigned char *pCodingBuf, 
                          _IN size_t bufMaxLen, 
                          _IN const EDCC_CFG_T &config)
{
    CHECK_POINTER_NULL_RETURN(pCodingBuf, 0);
    if(ptCoding != NULL)
    {
        memcpy(pCodingBuf, ptCoding, ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));
        return ptCoding->codingBuffLen + sizeof(EDCC_CODING_T);
    }
    
    size_t coding_size = encrypt(config);
    if(coding_size == 0
       || bufMaxLen < ptCoding->codingBuffLen + sizeof(EDCC_CODING_T)) {
        EDCC_Log("EDCCoding::encrypt bufMaxLen smaller than the real space occupied!\n");
        return 0;
    }
    memcpy(pCodingBuf, ptCoding, coding_size);
    
    return coding_size;
}


size_t EDCCoding::encrypt(_IN const EDCC_CFG_T &config)
{
    CHECK_NE_RETURN(ptCoding, NULL, ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));

    Check checkHadler;
    if(!checkHadler.checkConfigValid(config)) {
        EDCC_Log("EDCCoding::encrypt config error!\n");
        return 0;
    }
    initPtCoding(config);
    if(this->ptCoding == NULL) {
        return 0;
    }
    genCodingBytes();

    stringstream ss;
    size_t pos = 0;
    while(pos < ptCoding->codingBuffLen + sizeof(EDCC_CODING_T)) {
        char tmp[3];
        sprintf(tmp, "%02x", ((unsigned char*)ptCoding)[pos]);
        ss << tmp;
        ++pos;
    }

    return sizeof(EDCC_CODING_T) + ptCoding->codingBuffLen;
}

bool EDCCoding::decrypt(_IN const unsigned char *pCodingBuf)
{
    CHECK_POINTER_NULL_RETURN(pCodingBuf, false);

    const EDCC_CODING_T *l_ptCoding = (EDCC_CODING_T*)pCodingBuf;
    int actMagicKey = 0;
    memcpy(&actMagicKey, l_ptCoding->pCodingBuff + l_ptCoding->codingBuffLen - MAGIC_KEY_LEN, MAGIC_KEY_LEN);
    CHECK_NE_RETURN(actMagicKey, magicKey, false);

    initPtCoding(l_ptCoding->cfg);
    if(this->ptCoding == NULL) {
        EDCC_Log("EDCCoding::decrypt failed!\n");
        return false;
    }
    memcpy(this->ptCoding, l_ptCoding, l_ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));

    return true;
}

string EDCCoding::encodeToHexString(_IN const EDCC_CFG_T &config)
{
    string sRet = "";
    size_t bufMaxLen = config.imageSizeW * config.imageSizeH;
    unsigned char* pCoding = (unsigned char*)malloc(sizeof(unsigned char) * bufMaxLen);
    CHECK_POINTER_NULL_RETURN(pCoding, "");
    memset(pCoding, 0, sizeof(unsigned char) * bufMaxLen);
    size_t coding_size = encrypt(pCoding, bufMaxLen, config);
    if(coding_size == 0) {
        free(pCoding);
        pCoding = NULL;
        return "";
    }

    size_t pos = 0;
    stringstream ss;
    while(pos < coding_size) {
        char tmp[3];
        sprintf(tmp, "%02x", ((unsigned char*)pCoding)[pos]);
        ss << tmp;
        ++pos;
    }
    sRet = ss.str();
    free(pCoding);
    pCoding = NULL;

    return sRet;
}
bool EDCCoding::decodeFromHexString(_IN const string &hexString)
{
    size_t codingLen = hexString.length() / 2;
    CHECK_EQ_RETURN(codingLen, 0, false);
    unsigned char* pCoding = (unsigned char*)malloc(sizeof(unsigned char) * codingLen);
    CHECK_POINTER_NULL_RETURN(pCoding, false);
    memset(pCoding, 0, sizeof(unsigned char) * codingLen);
    
    for(size_t i = 0; i < codingLen; ++i) {
        string tmp = hexString.substr(i * 2, 2);
        sscanf(tmp.c_str(), "%02x", pCoding + i);
    }
    bool bRes = decrypt(pCoding);
    free(pCoding);
    pCoding = NULL;
    CHECK_EQ_RETURN(bRes, false, false);

    return true;
}

void EDCCoding::genCodingBytes()
{
    CHECK_POINTER_NULL_RETURN_VOID(ptCoding);
    memset(ptCoding->pCodingBuff, 0, ptCoding->codingBuffLen);

    size_t buffOffset = 0;
    int counter = 0;
    for(int h = 0; h < C.rows; ++h) {
        for(int w = 0; w < C.cols; ++w) {
            ++counter;
            unsigned char codingC = C.at<char>(h, w);
            if(counter % 2 != 0) {
                codingC <<= 4;
            }
            *(ptCoding->pCodingBuff + buffOffset) |= codingC;
            if(counter == C.rows*C.cols
               || counter % 2 == 0) {
                ++buffOffset;
            }
        }
    }

    counter = 0;
    for(int h = 0; h < Cs.rows; ++h) {
        for(int w = 0; w < Cs.cols; ++w) {
            unsigned char codingCs = Cs.at<char>(h, w);
            *(ptCoding->pCodingBuff + buffOffset) |= (codingCs << (7 - (counter % 8)));
            ++counter;
            if(counter == Cs.rows*Cs.cols
               || counter % 8 == 0)
            {
                ++buffOffset;
            }
        }
    }

    memcpy(ptCoding->pCodingBuff + buffOffset, &magicKey, MAGIC_KEY_LEN);
}

/*void EDCCoding::compressCoding()
{
    int width = C.cols;
    int height = C.rows;
    unsigned char comCodingCs = 0x00;
    int csCount = 3;
    stringstream ssZipC, ssZipCs;
    for(int h = 0; h < height; ++h) {
        for(int w = 0; w < width; ++w) {
            unsigned char codingC = C.at<char>(h, w);
            ssZipC << hexArray[codingC];
            unsigned char codingCs = Cs.at<char>(h, w);
            if(csCount == -1) {
                ssZipCs << hexArray[comCodingCs];
                csCount = 3;
                comCodingCs = 0x00;
            }
            comCodingCs |= (codingCs << csCount);
            --csCount;
        }
    }
    if(csCount != 3) {
        ssZipCs << hexArray[comCodingCs];
    }

    zipCodingC = ssZipC.str();
    zipCodingCs = ssZipCs.str();
}*/

bool EDCCoding::initPtCoding(_IN const EDCC_CFG_T &config)
{
    freeCoding();
    memcpy(&(this->cfg), &config, sizeof(EDCC_CFG_T));

    size_t imageSize = config.imageSizeW*config.imageSizeH;
    size_t t_coding_size = sizeof(EDCC_CODING_T) + (size_t)ceil(imageSize/2.0) + (size_t)ceil(imageSize/8.0) + MAGIC_KEY_LEN;
    this->ptCoding = (EDCC_CODING_T *)malloc(t_coding_size);
    if(this->ptCoding == NULL) {
        return false;
    }

    memset(this->ptCoding, 0, t_coding_size);
    memcpy(&(this->ptCoding->cfg), &config, sizeof(EDCC_CFG_T));
    ptCoding->codingBuffLen = t_coding_size - sizeof(EDCC_CODING_T);

    return true;
}


void EDCCoding::freeCoding()
{
    if(this->ptCoding != NULL) {
        free(this->ptCoding);
        this->ptCoding = NULL;
    }
    memset(&(this->cfg), 0, sizeof(EDCC_CFG_T));
}

//---------------------------------PalmprintCode--------------------------------

PalmprintCode::~PalmprintCode()
{
}

PalmprintCode& PalmprintCode::operator =(_IN const PalmprintCode &other)
{
    Palmprint::operator=(other);
    EDCCoding::operator=(other);

    return *this;
}

bool PalmprintCode::encodePalmprint(_IN const EDCC_CFG_T &config)
{
    bool bRet = false;
    Check checkHadler;
    if(!checkHadler.checkConfigValid(config)) {
        return false;
    }

    bRet = encodePalmprint(Size(config.imageSizeW, config.imageSizeH),
                           config.gaborSize,
                           config.directions,
                           config.laplaceSize);
    return bRet;
}

bool PalmprintCode::encodePalmprint(_IN const cv::Size &imgSize,
                                    _IN u_short gabKerSize,
                                    _IN u_char numOfDirections,
                                    _IN u_char lapKerSize)
{
    GaborFilter filter(cv::Size(gabKerSize, gabKerSize), numOfDirections);
    Mat *imgRet = genSpecImg(imgSize);
    if(imgRet == NULL) {
        EDCC_Log("%s not exists!\n", getImagePath().c_str());
        return false;
    }
    Mat palmprintImage = *imgRet;
    Mat gaborResult;
    enhanceImage(palmprintImage, palmprintImage, lapKerSize);
    filter.doGaborFilter(palmprintImage, gaborResult);
    vector<cv::Mat> resultVec;
    split(gaborResult, resultVec);
    genEDCCoding(resultVec, imgSize, numOfDirections);
    //compressCoding();

    return true;
}

void PalmprintCode::enhanceImage(_IN const cv::Mat &src, 
                                 _INOUT cv::Mat &dst,
                                 _IN u_char lapKerSize)
{
    Mat gaussian;
    GaussianBlur(src, gaussian, Size(5, 5), 0, 0, BORDER_DEFAULT);
    Laplacian(gaussian, dst, CV_64F, lapKerSize);
    normalize(dst, dst, 0, 1, CV_MINMAX);
}

void PalmprintCode::genEDCCoding(_IN const vector<cv::Mat> &filterResult,
                                 _IN const Size &imgSize,
                                 _IN u_char numOfDirections)
{
    this->C = Mat(imgSize, CV_8S);
    this->Cs = Mat(imgSize, CV_8S);
    for(int h = 0; h < imgSize.height; ++h) {
        for(int w = 0; w < imgSize.width; ++w) {
            double maxResponse = -DBL_MAX;
            int maxDirection = -1;
            int Cleft = -1, Cright = -1;
            for(u_char d = 0; d < numOfDirections; ++d) {
                double response = filterResult[d].at<double>(h, w); 
                if(response > maxResponse) {
                    maxResponse = response;
                    maxDirection = d;
                }
            }
            C.at<char>(h, w) = maxDirection;
            maxDirection == numOfDirections - 1 ? Cleft = 0 : Cleft = maxDirection + 1;
            maxDirection == 0 ? Cright = numOfDirections - 1 : Cright = maxDirection - 1;
            Cs.at<char>(h, w) = filterResult[Cleft].at<double>(h, w) >= 
                filterResult[Cright].at<double>(h, w) ? 1 : 0;
        }
    }
}

double PalmprintCode::matchWith(_IN const PalmprintCode &cmp) const 
{
    Match matchHandler;
    return matchHandler.matchP2P(*this, cmp);
}

//---------------------------------GaborFilter-----------------------------------

GaborFilter::GaborFilter(_IN const cv::Size &kernelSize,
                         _IN u_char numOfDirections)
{
    assert(kernelSize.width %2 == 1 && kernelSize.height % 2 == 1);
    assert(numOfDirections > 0);

    this->kernelSize = kernelSize;
    this->numOfDirections = numOfDirections;
    this->kernelType = kernelType;
}

GaborFilter::~GaborFilter()
{
}

void GaborFilter::doGaborFilter(_IN const cv::Mat &src, _INOUT cv::Mat &dstMerge)
{
    vector<cv::Mat> dstVec;
    Mat dst;
    int gaborH = this->kernelSize.height;
    int gaborW = this->kernelSize.width;
    Mat gaborKernel;
    for(u_char direction = 0; direction < this->numOfDirections; ++direction) {
        getGaborKernelReal(gaborKernel, gaborW, gaborH, 0, direction);
        filter2D(src, dst, CV_64F, gaborKernel);
        normalize(dst, dst, 0, 1, CV_MINMAX);
        dstVec.push_back(dst.clone());
    }
    merge(dstVec, dstMerge);
}

void GaborFilter::getGaborKernelReal(_INOUT cv::Mat &gaborKernel,
                                     _IN int kernelWidth, _IN int kernelHeight,
                                     _IN int dimension, _IN int direction,
                                     _IN double Kmax, _IN double f,
                                     _IN double sigma, _IN int ktype)
{
    assert(ktype == CV_32F || ktype == CV_64F);

    int halfWidth = kernelWidth / 2;
    int halfHeight = kernelHeight / 2;
    double Qu = CV_PI * direction / this->numOfDirections;
    double sqsigma = sigma * sigma;
    double Kv = Kmax / pow(f, dimension);
    double postmean = exp(-sqsigma / 2);
    Mat kernel(kernelWidth, kernelHeight, ktype);
    double tmp1, tmp2;
    for(int i = -halfHeight; i <= halfHeight; i++) {
        for(int j = -halfWidth; j <= halfWidth; j++) {
            tmp1 = exp(-(Kv * Kv * (i * i + j * j)) / (2 * sqsigma));
            tmp2 = cos(Kv * cos(Qu) * j + Kv * sin(Qu) * i) - postmean;
            if(ktype == CV_32F) {
                kernel.at<float>(i + halfHeight, j + halfWidth) =
                    (float)(Kv * Kv * tmp1 * tmp2 / sqsigma);
            } else {
                kernel.at<double>(i + halfHeight, j + halfWidth) =
                    (double)(Kv * Kv * tmp1 * tmp2 / sqsigma);
            }
        }
    }
    gaborKernel = kernel.clone();
}