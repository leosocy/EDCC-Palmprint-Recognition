/*************************************************************************
	> File Name: Core.cc
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/07/30 00:11:24
 ************************************************************************/
#include <Core.h>
using namespace EDCC;

//-----------------------------------Palmprint-------------------------------------

Palmprint::Palmprint()
{
	this->identity = "";
	this->imagePath = "";
}

Palmprint::Palmprint(const char *identity, const char *imagePath)
{
    bool bRet = false;
    bRet = setPalmprintInfo(identity, imagePath);
    if(bRet == false) {
        return;
    }
}


Palmprint::~Palmprint()
{
}

Palmprint& Palmprint::operator =(const Palmprint &src)
{
	this->identity = src.identity;
	this->imagePath = src.imagePath;
	this->image = src.image.clone();
	return *this;
}

bool Palmprint::setPalmprintInfo(const char *identity, const char *imagePath)
{
	if(identity == NULL || imagePath == NULL) {
        cerr << "Identity or imagePath can't be null!" << endl;
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
        cerr << "Read image failed!" << endl;
        return NULL;
    }
	return &image;
}

cv::Mat* Palmprint::genSpecImg(const cv::Size &imgSize, bool isGray)
{
    Mat *ptOriImg = genOriImg();
	if(ptOriImg == NULL) {
        return NULL;
    }
	resize(*ptOriImg, *ptOriImg, imgSize);
	if(isGray) {
		cvtColor(*ptOriImg, *ptOriImg, CV_BGR2GRAY);
	}
	return ptOriImg;
}

//---------------------------------EDCCoding------------------------------------
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
    zipCodingC = ssZipC.str();
    zipCodingCs = ssZipCs.str();
}

bool EDCCoding::decompressCoding(const char *ptCodingC, const char *ptCodingCs)
{
    return true;
}

//---------------------------------PalmprintCode--------------------------------

PalmprintCode::~PalmprintCode()
{
}

PalmprintCode& PalmprintCode::operator =(const PalmprintCode &src)
{
    this->C = src.C;
    this->Cs = src.Cs.clone();
    Palmprint::operator =(src);
    return *this;
}

bool PalmprintCode::encodePalmprint(const map< string, int > &configMap)
{
    bool bRet = false;
	if(configMap.find("imageSize") == configMap.end()
        || configMap.find("laplaceKernelSize") == configMap.end()
        || configMap.find("gaborKernelSize") == configMap.end()
        || configMap.find("gaborDirections") == configMap.end()) {
        cerr << "Config error!" << endl;
        return false;
    }
	bRet = encodePalmprint(Size(configMap.at("imageSize"), configMap.at("imageSize")), 
					Size(configMap.at("gaborKernelSize"), configMap.at("gaborKernelSize")), 
					configMap.at("gaborDirections"), 
					Size(configMap.at("laplaceKernelSize"), configMap.at("laplaceKernelSize")));
    return bRet;
}

bool PalmprintCode::encodePalmprint(const cv::Size &imgSize, 
                                            const cv::Size &gabKerSize, 
                                            int numOfDirections, 
                                            const cv::Size &lapKerSize) 
{
	if(lapKerSize.width % 2 != 1 && lapKerSize.width != lapKerSize.height) {
        cerr << "The width and height of laplace kernel mast be odd!" << endl;
        return false;
	}
	GaborFilter filter(gabKerSize, numOfDirections, GaborFilter::GABOR_KERNEL_REAL);
    Mat *imgRet = genSpecImg(imgSize);
    if(imgRet == NULL) {
        cerr << imagePath << " not exists!\n";
        return false;
    }
	Mat palmprintImage = *imgRet;
	Mat gaborResult;
	enhanceImage(palmprintImage, palmprintImage, lapKerSize);
	filter.doGaborFilter(palmprintImage, gaborResult);
	vector<cv::Mat> resultVec;
	split(gaborResult, resultVec);
	genEDCCoding(resultVec, imgSize, numOfDirections);
    compressCoding();
    return true;
}

void PalmprintCode::enhanceImage(const cv::Mat &src, cv::Mat &dst, const cv::Size &lapKerSize)
{
	Mat gaussian;
	GaussianBlur(src, gaussian, Size(5, 5), 0, 0, BORDER_DEFAULT);
	Laplacian( gaussian, dst, CV_64F, lapKerSize.width);
	normalize(dst, dst, 0, 1, CV_MINMAX);
}

void PalmprintCode::genEDCCoding(const vector<cv::Mat> &filterResult, 
                                        const Size &imgSize, 
                                        int numOfDirections)
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


//---------------------------------GaborFilter-----------------------------------

GaborFilter::GaborFilter(const cv::Size &kernelSize, int numOfDirections, int kernelType)
{
	assert(kernelSize.width %2 == 1 && kernelSize.height % 2 == 1);
	assert(kernelType == GaborFilter::GABOR_KERNEL_REAL 
            || kernelType == GaborFilter::GABOR_KERNEL_IMAG 
            || kernelType == GaborFilter::GABOR_KERNEL_MAG);
	assert(numOfDirections > 0);
	this->kernelSize = kernelSize;
	this->numOfDirections = numOfDirections;
	this->kernelType = kernelType;
}

GaborFilter::~GaborFilter()
{
}

void GaborFilter::doGaborFilter(const cv::Mat &src, cv::Mat &dstMerge)
{
	vector<cv::Mat> dstVec;
	Mat dst;
	int gaborH = this->kernelSize.height;
	int gaborW = this->kernelSize.width;
	Mat gaborKernel;
	for(int direction = 0; direction < this->numOfDirections; ++direction) {
		getGaborKernel(gaborKernel, gaborW, gaborH, 0, direction, this->kernelType);
		filter2D(src, dst, CV_64F, gaborKernel);
		normalize(dst, dst, 0, 1, CV_MINMAX);
		dstVec.push_back(dst.clone());
	}
	merge(dstVec, dstMerge);
}

void GaborFilter::getGaborKernel(cv::Mat &gaborKernel, int kernelWidth, int kernelHeight,
		                            int dimension, int direction, 
		                            int kernelType, double Kmax, double f,
		                            double sigma, int ktype)
{
    assert(ktype == CV_32F || ktype == CV_64F);  
    int halfWidth = kernelWidth / 2;
    int halfHeight = kernelHeight / 2;
    double Qu = CV_PI * direction / this->numOfDirections;
    double sqsigma = sigma * sigma;
    double Kv = Kmax / pow(f, dimension);
    double postmean = exp(-sqsigma / 2);
    Mat kernel(kernelWidth, kernelHeight, ktype);
    Mat kernel_2(kernelWidth, kernelHeight, ktype);
    Mat kernel_mag(kernelWidth, kernelHeight, ktype);
    double tmp1, tmp2, tmp3;
    for(int j = -halfHeight; j <= halfHeight; j++) {
        for(int i = -halfWidth; i <= halfWidth; i++) {
            tmp1 = exp(-(Kv * Kv * (j * j + i * i )) / (2 * sqsigma));
            tmp2 = cos(Kv * cos(Qu) * i + Kv * sin(Qu) * j) - postmean;
            tmp3 = sin(Kv * cos(Qu) * i + Kv * sin(Qu) * j);
            if(kernelType == GaborFilter::GABOR_KERNEL_REAL) {
                if(ktype == CV_32F) {
                    kernel.at<float>(j + halfHeight, i + halfWidth) =
                        (float)(Kv * Kv * tmp1 * tmp2 / sqsigma);
                } else {
                    kernel.at<double>(j + halfHeight, i + halfWidth) =
                        (double)(Kv * Kv * tmp1 * tmp2 / sqsigma);
                }
            } else if(kernelType == GaborFilter::GABOR_KERNEL_IMAG) {
                if(ktype == CV_32F) {
                    kernel.at<float>(j + halfHeight, i + halfWidth) =
                        (float)(Kv * Kv * tmp1 * tmp3 / sqsigma);
                } else {
                    kernel.at<double>(j + halfHeight, i + halfWidth) =
                        (double)(Kv * Kv * tmp1 * tmp3 / sqsigma);
                }
            } else {
                if(ktype == CV_32F) {
                    kernel.at<float>(j + halfHeight, i + halfWidth) =
                        (float)(Kv * Kv * tmp1 * tmp2 / sqsigma);
                    kernel_2.at<float>( j + halfHeight, i + halfWidth) =
                        (float)(Kv * Kv * tmp1 * tmp3 / sqsigma);
                } else {
                    kernel.at<double>(j + halfHeight, i + halfWidth) =
                        (double)(Kv * Kv * tmp1 * tmp2 / sqsigma);
                    kernel_2.at<double>(j + halfHeight, i + halfWidth) =
                        (double)(Kv * Kv * tmp1 * tmp3 / sqsigma);
                }		
            }
        }
	}
    if(kernelType == GaborFilter::GABOR_KERNEL_MAG) {
        magnitude(kernel, kernel_2, kernel_mag);
        gaborKernel = kernel_mag.clone();
    } else {
        gaborKernel = kernel.clone();
    }
}
