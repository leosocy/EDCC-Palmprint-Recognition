/*************************************************************************
	> File Name: palmprint_identify.cc
	> Author: Leosocy
	> Mail: 513887568@qq.com 
	> Created Time: 2017/07/26 21:27:26
 ************************************************************************/
#include <EDCC/IO.h>

using namespace EDCC;
int main(int argc, const char **argv)
{
	ifstream configIn; 
	configIn.open("config.json");
	map< string, int >config;
	IO::loadConfig( configIn, config );
	return 0;
}
