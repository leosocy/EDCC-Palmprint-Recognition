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
	map< string, int >config;
	IO::loadConfig( configIn, config );

	ifstream groupIn;
	groupIn.open("palmprintTrainGroup.json");
	vector< Palmprint > groupVec;
	IO::loadPalmprintGroup( groupIn, groupVec );
	for( int index = 0; index < groupVec.size(); ++index ) {
		PalmprintCode instance( groupVec[index] );
		instance.encodePalmprint( Size( config["imageSize"], config["imageSize"]), 
				Size( config["gaborKernelSize"], config["gaborKernelSize"] ), 
				config["gaborDirections"], 
				Size( config["laplaceKernelSize"], config["laplaceKernelSize"] ) );
		int i =  instance.genCoding().C.at<char>( 0, 5 );
		cout << i << endl;
	}
	return 0;
}
