/*************************************************************************
	> File Name: IO.h
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/07/23 18:02:11
 ************************************************************************/

#ifndef __IO_h__
#define __IO_h__
#include <opencv2/opencv.hpp>
#include <fstream>
namespace EDCC
{
	//=========================================================
	/*
	 *Input & Output Module 
	 */

	class IO {
		public:
			static void loadConfig( const char *configFileName );
			static void loadPalmprintGroup( const char *groupFileName );
			static void loadPalmprintFeatureData( const char *dataFileName );
			static void savePalmprintFeatureData( const char *dataFileName, vector< Core > data );
	};
	//=========================================================
}

#endif 
