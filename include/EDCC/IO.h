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
#include <set>
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
			map< string, int > paramsMap;
			
			IO();

			int loadConfig( ifstream &in  );
			int loadPalmprintGroup( ifstream &in, vector< Palmprint > &groupVec );
			int loadPalmprintFeatureData( ifstream &in, vector< PalmprintCode > &data );
			int savePalmprintFeatureData( ofstream &out, vector< PalmprintCode > &data );
		private:
			set< string > paramsSet;
			int loadOneIdentityAllPalmprintFeatureData( const string &identity, const Json::Value &value, vector< PalmprintCode > &data );			
			void genEDCCoding( const Json::Value &value, EDCCoding &coding );
			void jsonArray2Mat( const Json::Value &value, Mat &dst );
			bool insert2JsonValue( PalmprintCode &code, Json::Value &value );
			void setEDCCoding( const EDCCoding &coding, Json::Value &value );
			void Mat2jsonArray( const Mat &src, Json::Value &value );
	};
	//=========================================================
}

#endif 
