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
    m_ptCoding = NULL;
    magicKey = 0x0622520a;
}

EDCCoding::~EDCCoding()
{
    freeSelfCoding();
}

EDCCoding& EDCCoding::operator =(_IN const EDCCoding &other)
{
    if(this != &other) {
        this->C = other.C.clone();
        this->Cs = other.Cs.clone();
        this->magicKey = other.magicKey;
        freeSelfCoding();
        if(other.m_ptCoding != NULL) {
            this->m_ptCoding = (EDCC_CODING_T*)malloc(other.m_ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));
            memcpy(this->m_ptCoding, other.m_ptCoding, other.m_ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));
        }
    }

    return *this;
}

EDCCoding::EDCCoding(_IN const EDCCoding &other)
{
    this->C = other.C.clone();
    this->Cs = other.Cs.clone();
    this->magicKey = other.magicKey;
    this->m_ptCoding = NULL;
    if(other.m_ptCoding != NULL) {
        this->m_ptCoding = (EDCC_CODING_T*)malloc(other.m_ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));
        memcpy(this->m_ptCoding, other.m_ptCoding, other.m_ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));
    }
}

size_t EDCCoding::encrypt(_INOUT unsigned char *pCodingBuf, 
                          _IN size_t bufMaxLen, 
                          _IN const EDCC_CFG_T &config)
{
    CHECK_POINTER_NULL_RETURN(pCodingBuf, 0);
    if(m_ptCoding != NULL)
    {
        if(bufMaxLen < m_ptCoding->codingBuffLen + sizeof(EDCC_CODING_T)) {
            EDCC_Log("EDCCoding::encrypt bufMaxLen smaller than the real space occupied!\n");
            return 0;
        }
        memcpy(pCodingBuf, m_ptCoding, m_ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));
        return m_ptCoding->codingBuffLen + sizeof(EDCC_CODING_T);
    }
    
    size_t codingLen = encrypt(config);
    if(codingLen == 0
       || bufMaxLen < codingLen) {
        EDCC_Log("EDCCoding::encrypt bufMaxLen smaller than the real space occupied!\n");
        return 0;
    }
    memcpy(pCodingBuf, m_ptCoding, codingLen);
    
    return codingLen;
}


size_t EDCCoding::encrypt(_IN const EDCC_CFG_T &config)
{
    CHECK_NE_RETURN(m_ptCoding, NULL, m_ptCoding->codingBuffLen + sizeof(EDCC_CODING_T));

    Check checkHadler;
    if(!checkHadler.checkConfigValid(config)) {
        EDCC_Log("EDCCoding::encrypt config error!\n");
        return 0;
    }
    initSelfCoding(config);
    CHECK_POINTER_NULL_RETURN(m_ptCoding, 0);

    genCodingBytes();

    return sizeof(EDCC_CODING_T) + m_ptCoding->codingBuffLen;
}

bool EDCCoding::decrypt(_IN const unsigned char *pCodingBuf)
{
    CHECK_POINTER_NULL_RETURN(pCodingBuf, false);
    Check checkHandler;

    const EDCC_CODING_T *l_ptCoding = (EDCC_CODING_T*)pCodingBuf;
    size_t codingLen = l_ptCoding->codingBuffLen + sizeof(EDCC_CODING_T);
    int actMagicKey = 0;
    memcpy(&actMagicKey, l_ptCoding->pCodingBuff + l_ptCoding->codingBuffLen - MAGIC_KEY_LEN, MAGIC_KEY_LEN);
    CHECK_NE_RETURN(actMagicKey, magicKey, false);

    initSelfCoding(l_ptCoding->cfg);
    if(m_ptCoding == NULL) {
        EDCC_Log("EDCCoding::decrypt failed!\n");
        return false;
    }
    memcpy(m_ptCoding, l_ptCoding, codingLen);

    return checkHandler.checkCoding(*this);
}

string EDCCoding::encryptToHexString(_IN const EDCC_CFG_T &config)
{
    string sRet = "";
    size_t bufMaxLen = config.imageSizeW * config.imageSizeH;
    size_t coding_size = encrypt(config);
    if(coding_size == 0) {
        return "";
    }

    size_t pos = 0;
    stringstream ss;
    while(pos < coding_size) {
        char tmp[3];
        sprintf(tmp, "%02x", ((unsigned char*)m_ptCoding)[pos]);
        ss << tmp;
        ++pos;
    }
    sRet = ss.str();

    return sRet;
}
bool EDCCoding::decryptFromHexString(_IN const string &hexString)
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
    CHECK_POINTER_NULL_RETURN_VOID(m_ptCoding);
    memset(m_ptCoding->pCodingBuff, 0, m_ptCoding->codingBuffLen);

    size_t buffOffset = 0;
    int counter = 0;
    for(int h = 0; h < C.rows; ++h) {
        for(int w = 0; w < C.cols; ++w) {
            ++counter;
            unsigned char codingC = C.at<char>(h, w);
            if(counter % 2 != 0) {
                codingC <<= 4; 
                codingC &= 0xf0;
            }
            *(m_ptCoding->pCodingBuff + buffOffset) |= codingC;
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
            *(m_ptCoding->pCodingBuff + buffOffset) |= (codingCs << (7 - (counter % 8)));
            ++counter;
            if(counter == Cs.rows*Cs.cols
               || counter % 8 == 0)
            {
                ++buffOffset;
            }
        }
    }

    memcpy(m_ptCoding->pCodingBuff + buffOffset, &magicKey, MAGIC_KEY_LEN);
}

void EDCCoding::initSelfCoding(_IN const EDCC_CFG_T &config)
{
    freeSelfCoding();

    size_t imageSize = config.imageSizeW*config.imageSizeH;
    size_t codingLen = sizeof(EDCC_CODING_T) 
                        + (size_t)ceil(imageSize / 2.0) 
                        + (size_t)ceil(imageSize / 8.0) 
                        + MAGIC_KEY_LEN;
    m_ptCoding = (EDCC_CODING_T *)malloc(codingLen);
    CHECK_POINTER_NULL_RETURN_VOID(m_ptCoding);
    memset(m_ptCoding, 0, codingLen);
    
    memcpy(&(m_ptCoding->cfg), &config, sizeof(EDCC_CFG_T));
    m_ptCoding->codingBuffLen = codingLen - sizeof(EDCC_CODING_T);
}

void EDCCoding::freeSelfCoding()
{
    if(m_ptCoding != NULL) {
        free(m_ptCoding);
        m_ptCoding = NULL;
    }
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
        normalize(gaborKernel, gaborKernel, 0, 1, CV_MINMAX);
        filter2D(src, dst, CV_64F, gaborKernel);
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
    for(int i = -halfHeight; i <= halfHeight; ++i) {
        for(int j = -halfWidth; j <= halfWidth; ++j) {
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
