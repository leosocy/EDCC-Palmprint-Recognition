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


int IO::loadConfig( ifstream &in, std::map< string, int > &configMap )
{
	assert( in.is_open() );
	Json::Value value;
	Json::Reader reader;
	Json::Value::iterator iter;
	Json::Value::Members members;

	if( !reader.parse( in, value ) ) {
		cerr << "Parse config.json filed, please confirm the format." << endl;
		return LOAD_CONFIG_FAILURE;
	}
	members = value.getMemberNames();
	for( Json::Value::Members::iterator it = members.begin(); 
			it != members.end(); ++it ) {
		Json::Value::Members membersSub = value[*it].getMemberNames();
		Json::Value::Members::iterator itSub;
		for( itSub = membersSub.begin(); itSub != membersSub.end(); ++itSub ) {
			if( *itSub == "default" ) {
				configMap.insert( map< string, int >::value_type( *it, value[*it]["default"].asInt() ) );
				break;
			}
		}
		if( itSub == membersSub.end() ) { 
			cerr << "Parse config.json failed, you can only change the \"default\" \
				label in this file." << endl;
			return LOAD_CONFIG_FAILURE;
		}
	}

	for( map< string, int >::iterator it = configMap.begin(); it != configMap.end();
			++it ) {
		cout << (*it).first << ":" << (*it).second << endl;
	}
	return LOAD_CONFIG_SUCCESS;
}

#if 0
void loadPalmprintGroup( ifstream &in, vector< std::map< string, string > > groupVec )
{

}

static void loadPalmprintFeatureData( ifstream &in  )
{
}

static void savePalmprintFeatureData( ofstream &out )
{
}

#endif
