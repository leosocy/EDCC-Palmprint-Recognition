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
IO::IO()
{
	paramsSet.insert( "imageSize" );
	paramsSet.insert( "laplaceKernelSize" );
	paramsSet.insert( "gaborKernelSize" );
	paramsSet.insert( "gaborDirections" );
}

int IO::loadConfig( ifstream &in  )
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
			if( paramsSet.find( *it ) != paramsSet.end() ) {
				paramsMap.insert( map< string, int >::value_type( *it, value[*it]["default"].asInt() ) ); 
			} else {
				cerr << "Ilegal configuration parameters." << endl;
				return LOAD_CONFIG_FAILURE;
			}
		}
	}

	for( map< string, int >::iterator it = paramsMap.begin(); it != paramsMap.end();
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

int IO::loadPalmprintFeatureData( ifstream &in, vector< PalmprintCode > &data  )
{
	assert( in.is_open() );
	Json::Value value;
	Json::Reader reader;
	Json::Value::Members members;

	if( !reader.parse( in, value ) ) {
		cerr << "Parse json failed. Don't change the trainData.json format." << endl;
		return LOAD_PALMPRINT_FEATURE_DATA_FAILURE;
	}
	for( set< string >::iterator it = paramsSet.begin(); it != paramsSet.end(); ++it ) {
		if( value[*it].isNull() || !value[*it].isInt() ) {
			cerr << "Parse json failed. Don't change the trainData.json format." << endl;
			return LOAD_PALMPRINT_FEATURE_DATA_FAILURE;
		}
	}
	members = value.getMemberNames();
	for( Json::Value::Members::iterator it = members.begin(); 
			it != members.end(); ++it ) {
		if( paramsSet.find( *it ) != paramsSet.end() ) {
			paramsMap.insert( map< string, int >::value_type( *it, value[*it]["default"].asInt() ) ); 
		} else {
			loadOneIdentityAllPalmprintFeatureData( *it, value[*it], data );	
		}
	}

}

int IO::savePalmprintFeatureData( ofstream &out, const vector< PalmprintCode > &data )
{
	return SAVE_PALMPRINT_FEATURE_DATA_SUCCESS;
}

int IO::loadOneIdentityAllPalmprintFeatureData( const string &identity, const Json::Value &value, vector< PalmprintCode > &data )
{
	Json::Value::Members imagePathMembers;
	imagePathMembers = value.getMemberNames();
	for( Json::Value::Members::iterator it = imagePathMembers.begin(); 
			it != imagePathMembers.end(); ++it ) {
		Palmprint instance( identity, *it );
		EDCCoding coding;
		genEDCCoding( value[*it], coding );
		PalmprintCode instanceCode( instance );
		instanceCode.coding = coding;
		data.push_back( instanceCode );
	}
}

void IO::genEDCCoding( const Json::Value &value, EDCCoding &coding )
{
	assert( !value.isNull() );
	Mat C( paramsMap["imageSize"], paramsMap["imageSize"], CV_64F );
	Mat Cs( paramsMap["imageSize"], paramsMap["imageSize"], CV_64F );
	Json::Value cValue = value["C"];
	Json::Value csValue = value["Cs"];

	assert( cValue.size() == paramsMap["imageSize"] );
	assert( csValue.size() == paramsMap["imageSize"] );
	jsonArray2Mat( cValue, C );
	jsonArray2Mat( csValue, Cs );

	coding.C = C.clone();
	coding.Cs = Cs.clone();
}

int IO::jsonArray2Mat( const Json::Value &value, Mat &dst )
{
	assert( !value.isNull() && value.isArray() );
	for( int row = 0; row < value.size(); ++row ) {
		Json::Value colValue = value[row];
		assert( colValue.size() == paramsMap["imageSize"] );
		for( int col = 0; col < colValue.size(); ++col ) {
			dst.at<double>( row, col ) = colValue[col].asDouble();
		}
	}
	return EXIT_SUCCESS;
}
