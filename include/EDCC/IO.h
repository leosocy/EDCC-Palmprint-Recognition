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
#include <json/json.h>
#include <EDCC/Core.h>
using namespace std;
using namespace cv;

#define LOAD_CONFIG_FAILURE 1
#define LOAD_CONFIG_SUCCESS 0

#define LOAD_PALMPRINT_GROUP_FAILURE 1
#define LOAD_PALMPRINT_GROUP_SUCCESS 0

#define LOAD_PALMPRINT_FEATURE_DATA_FAILURE 1
#define LOAD_PALMPRINT_FEATURE_DATA_SUCCESS 0

#define SAVE_PALMPRINT_FEATURE_DATA_FAILURE 1
#define SAVE_PALMPRINT_FEATURE_DATA_SUCCESS 0

namespace EDCC
{
	//=========================================================
	/*
	 *Input & Output Module 
	 */
	class IO {
		public:
			static int loadConfig( const ifstream &in, std::map< string, int > &configMap );
			static int loadPalmprintGroup( const ifstream &in, vector< Palmprint > &groupVec );
			static int loadPalmprintFeatureData( const ifstream &in, vector< PalmprintCode > &data );
			static int savePalmprintFeatureData( ofstream &out, const vector< PalmprintCode > &data );
	};
	//=========================================================
}

#endif 
