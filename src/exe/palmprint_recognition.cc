/*************************************************************************
	> File Name: palmprint_identify.cc
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/07/26 21:27:26
 ************************************************************************/
#include <EDCC/EDCC.h>
using namespace EDCC;
int main(int argc, const char **argv)
{
	ifstream configIn; 
	configIn.open("config.json");
	IO trainIO;
	trainIO.loadConfig( configIn  );

	ifstream groupIn;
	groupIn.open("palmprintTrainGroup.json");
	vector< Palmprint > groupVec;
	vector< PalmprintCode > data;
	trainIO.loadPalmprintGroup( groupIn, groupVec );
	for( int index = 0; index < groupVec.size(); ++index ) {
		PalmprintCode one( groupVec[index] );
		one.encodePalmprint( trainIO.paramsMap );
		//data.push_back( one );
	}

	ifstream dataOut;
	dataOut.open( "traintrain.json" );
	trainIO.loadPalmprintFeatureData( dataOut, data );
	for( int index = 0; index < data.size(); ++index ) {
		cout << data[index].palmprint.identity << endl;
	}
	return 0;
}
