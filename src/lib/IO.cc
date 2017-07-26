#include <include/EDCC/IO.h>
#include <iostream>
#include <assert.h>
using namespace EDCC;
using namespace std;
using namespace cv;

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
    
