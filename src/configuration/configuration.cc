 ///
 /// @file    configuration.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2017-07-18 17:38:30
 ///

#include "configuration.h"
#include <unistd.h>
namespace cc
{
Configuration::Configuration(const string & confFilePath)
:_confFilePath(confFilePath)
{
	std::ifstream ifs;
	ifs.open(_confFilePath);
	if(!ifs.good()){
		cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:    ";
		cout<<"open confFile error"<<endl;	
		ifs.close();
		exit(EXIT_FAILURE);
	}
	string line,item,value;
	while(getline(ifs,line)){
		std::istringstream iss(line);
		iss>>item>>value;	
		_confMap.insert(std::map<string,string>::value_type(item,value));
	}
	ifs.close();
}

map<string,string> & Configuration::getConfMap(){
	return _confMap;
}

}//namespace cc
