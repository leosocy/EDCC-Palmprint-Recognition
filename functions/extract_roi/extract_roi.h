/*************************************************************************
	> File Name: extract_roi.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2016/12/21  13:03:11
 ************************************************************************/

#ifndef __EXTRACT_ROI_H__
#define __EXTRACT_ROI_H__


#include "./peak_valley_detect.h"
#include "../../global/global_definition.h"
#include "../../global/general_function.h"

/*
 *
 * return : EXIT_SUCCESS -- extract success			EXIT_FAILURE -- extract fail
 */

int extract_roi( const Mat &src, Mat &roi );



#endif 
