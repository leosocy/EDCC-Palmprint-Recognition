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

    bool bRet = false;
    bRet = setPalmprintInfo(identity, imagePath);
    if(bRet == false) {
        return ;
    }
}


Palmprint::~Palmprint()
{
}

Palmprint& Palmprint::operator =(_IN const Palmprint &src)
{
    this->identity = src.identity;
    this->imagePath = src.imagePath;
    this->image = src.image.clone();
    return *this;
}

bool Palmprint::setPalmprintInfo(_IN const char *identity, _IN const char *imagePath)
{
    if(identity == NULL || imagePath == NULL) {
        EDCC_Log("Identity or imagePath can't be null!\n");
        return false;
    }
    this->identity = identity;
    this->imagePath = imagePath;
    return true;
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


unsigned char* EDCCoding::encrypt()
{
    compressCoding();
    if(zipCodingC.empty()
       || zipCodingCs.empty())
    {
        return "";
    }
    string comCoding = zipCodingC + "\n" + zipCodingCs;

    /*AES aes;
    std::string errMsg;
    std::string outData;
    aes.encrypt4aes(comCoding, "062206271314520a", outData, errMsg);
    return errMsg == "" ? outData : "";*/
    return comCoding;
}

void EDCCoding::decrypt(const string &aesCoding)
{
    std::string strInput, errMsg;
    AES aes;
    aes.decrypt4aes(aesCoding, "062206271314520a", strInput, errMsg);
    size_t pos = strInput.find("\n");
    zipCodingC = strInput.substr(0, pos);
    zipCodingCs = strInput.substr(pos + 1, strInput.length() - pos - 1);
    cout << "C:" << zipCodingC << endl << "Cs:" << zipCodingCs << endl;
}

void EDCCoding::compressCoding()
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
}


//---------------------------------PalmprintCode--------------------------------

PalmprintCode::~PalmprintCode()
{
}

PalmprintCode& PalmprintCode::operator =(_IN const PalmprintCode &src)
{
    this->C = src.C;
    this->Cs = src.Cs.clone();
    Palmprint::operator =(src);
    return *this;
}

bool PalmprintCode::encodePalmprint(_IN const map< string, int > &configMap)
{
    bool bRet = false;
    Check checkHadler;
    if(!checkHadler.checkConfigValid(configMap)) {
        return false;
    }

    bRet = encodePalmprint(Size(configMap.at(IMAGE_SIZE_W), configMap.at(IMAGE_SIZE_H)),
                           configMap.at(GABOR_KERNEL_SIZE),
                           configMap.at(GABOR_DIRECTIONS),
                           configMap.at(LAPLACE_KERNEL_SIZE));
    return bRet;
}

bool PalmprintCode::encodePalmprint(_IN const cv::Size &imgSize,
                                    _IN int gabKerSize,
                                    _IN int numOfDirections,
                                    _IN int lapKerSize)
{
    GaborFilter filter(gabKerSize, numOfDirections);
    Mat *imgRet = genSpecImg(imgSize);
    if(imgRet == NULL) {
        EDCC_Log("%s not exists!\n", imagePath.c_str());
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
                                 _IN const cv::Size &lapKerSize)
{
    Mat gaussian;
    GaussianBlur(src, gaussian, Size(5, 5), 0, 0, BORDER_DEFAULT);
    Laplacian(gaussian, dst, CV_64F, lapKerSize.width);
    normalize(dst, dst, 0, 1, CV_MINMAX);
}

void PalmprintCode::genEDCCoding(_IN const vector<cv::Mat> &filterResult,
                                 _IN const Size &imgSize,
                                 _IN int numOfDirections)
{
    this->C = Mat(imgSize, CV_8S);
    this->Cs = Mat(imgSize, CV_8S);
    for(int h = 0; h < imgSize.height; ++h) {
        for(int w = 0; w < imgSize.width; ++w) {
            double maxResponse = -DBL_MAX;
            int maxDirection = -1;
            int Cleft = -1, Cright = -1;
            for(int d = 0; d < numOfDirections; ++d) {
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
                         _IN int numOfDirections)
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
    for(int direction = 0; direction < this->numOfDirections; ++direction) {
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