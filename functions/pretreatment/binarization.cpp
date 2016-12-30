/*************************************************************************
	> File Name: binarization.cpp
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/13  19:25:21
 ************************************************************************/

#include "binarization.h"


void cvSkinOtsu( const Mat &src, Mat &dst );
void cvThresholdOtsu( const Mat &frame, Mat &skinArea );

void binarization( const Mat &src, Mat &dst )
{
//	GaussianBlur( src, dst, cv::Size(5, 5), 1.5 );
//	imshow( "gauss ", dst );
	cvSkinOtsu( src, dst );
	//cvThresholdOtsu( src, dst );
}

void cvSkinOtsu( const Mat &src, Mat &dst )
{ 
	Mat ycrcb;
	Mat vector_mat[3];
	cvtColor( src, ycrcb, CV_BGR2YCrCb );
	split( ycrcb, vector_mat );
 //	cvThresholdOtsu( vector_mat + 1, dst );
#ifdef _DEBUG_
	//imshow( "Y", vector_mat[1] );
#endif
	threshold( vector_mat[1], dst, 0, 255, CV_THRESH_OTSU );
//	cvThresholdOtsu( src, dst );
}

void cvThresholdOtsu( const Mat &frame, Mat &skinArea )
{
	Mat YCbCr;  
    vector<Mat> planes;  
  
    //转换为YCrCb颜色空间  
    cvtColor(frame, YCbCr, CV_RGB2YCrCb);  
    //将多通道图像分离为多个单通道图像  
    split(YCbCr, planes);   
  
    //运用迭代器访问矩阵元素  
    MatIterator_<uchar> it_Cb = planes[1].begin<uchar>(),  
                        it_Cb_end = planes[1].end<uchar>();  
    MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();  
    MatIterator_<uchar> it_skin = skinArea.begin<uchar>();  
  
    //人的皮肤颜色在YCbCr色度空间的分布范围:100<=Cb<=127, 138<=Cr<=170  
    for( ; it_Cb != it_Cb_end; ++it_Cr, ++it_Cb, ++it_skin)  
    {  
        if (138 <= *it_Cr &&  *it_Cr <= 170 && 100 <= *it_Cb &&  *it_Cb <= 127)  
            *it_skin = 255;  
        else  
            *it_skin = 0;  
    }  
  
    //膨胀和腐蚀，膨胀可以填补凹洞（将裂缝桥接），腐蚀可以消除细的凸起（“斑点”噪声）  
    dilate(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1));  
    erode(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1));  
}
