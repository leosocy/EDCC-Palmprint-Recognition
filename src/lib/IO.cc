/*************************************************************************
	> File Name: IO..cc
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/07/26 21:29:10
 ************************************************************************/
#include <EDCC/IO.h>
#include <iostream>
#include <assert.h>
using namespace EDCC;


void loadConfig( ifstream &in, vector< std::map< string, string > > configVec )
{
  assert( in.is_open() );
  string line, configKey, configValue;
  while( getline( in, line ) ) {
    line >> configKey >> configValue;
    configVec.push
  }
}


void loadPalmprintGroup( ifstream &in, vector< std::map< string, string > > groupVec )
{

}

static void loadPalmprintFeatureData( ifstream &in  )
{
}

static void savePalmprintFeatureData( ofstream &out )
{
}
