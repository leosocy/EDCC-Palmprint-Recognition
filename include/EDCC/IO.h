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
#include <vector>
#include <map>
#include <string>
using namespace std;
using namespace cv;
namespace EDCC
{
	//=========================================================
	/*
	 *Input & Output Module 
	 */
	class Palmprint;
	class IO {
		public:
			static void loadConfig( ifstream &in, vector< std::map< string, string > > configVec );
			static void loadPalmprintGroup( ifstream &in, vector< Palmprint > groupVec );
			static void loadPalmprintFeatureData( ifstream &in  );
			static void savePalmprintFeatureData( ofstream &out );
	};
	//=========================================================
}

#endif 
