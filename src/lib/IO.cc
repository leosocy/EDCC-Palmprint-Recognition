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

#define PALMPRINT_GROUP_FORMAT  "{\n\
\"identity\" : [\n\
	\"path1\"\n\
	\"path2\"\n\
	]\n\
}"

int IO::loadConfig( ifstream &in, map< string, int > &configMap )
{
	assert( in.is_open() );
	Json::Value value;
	Json::Reader reader;
	Json::Value::Members members;

	if( !reader.parse( in, value ) ) {
		cerr << "Parse config.json failed, please confirm the format." << endl;
		return LOAD_CONFIG_FAILURE;
	}
	members = value.getMemberNames();
	for( Json::Value::Members::iterator it = members.begin(); 
			it != members.end(); ++it ) {
		if( value[*it]["default"].isNull() ) {
			cerr << "Parse config.json failed, you can only change the value\
 of \"default\" label in this file." << endl;
			return LOAD_CONFIG_FAILURE;
		} else {
			configMap.insert( map< string, int >::value_type( *it, value[*it]["default"].asInt() ) );
		}
	}

	for( map< string, int >::iterator it = configMap.begin(); it != configMap.end();
			++it ) {
		cout << (*it).first << ":" << (*it).second << endl;
	}
	return LOAD_CONFIG_SUCCESS;
}

int IO::loadPalmprintGroup( ifstream &in, vector< Palmprint > &groupVec )
{
	assert( in.is_open() );
	Json::Value value;
	Json::Reader reader;
	Json::Value::Members members;

	if( !reader.parse( in, value ) ) {
		cerr << "Parse json failed. Don't change the json format. You need to confirm the format like this." << endl;
		cerr << PALMPRINT_GROUP_FORMAT << endl;
		return LOAD_PALMPRINT_GROUP_FAILURE;
	}
	members = value.getMemberNames();
	for( Json::Value::Members::iterator it = members.begin(); 
			it != members.end(); ++it ) {
		if( !value[*it].isArray() ) {
			cerr << "Don't change the json format. You need to confirm the format like this." << endl;
			cerr << PALMPRINT_GROUP_FORMAT << endl;
			return LOAD_PALMPRINT_GROUP_FAILURE;
		}
		Json::Value imageList = value[*it];
		for( int imageIndex = 0; imageIndex < imageList.size(); ++imageIndex ) {
			Palmprint newOne( *it, imageList[imageIndex].asString() );
			groupVec.push_back( newOne );
		}
	}
	return LOAD_PALMPRINT_GROUP_SUCCESS;
}

#if 0
static void loadPalmprintFeatureData( ifstream &in  )
{
}

static void savePalmprintFeatureData( ofstream &out )
{
}

#endif
