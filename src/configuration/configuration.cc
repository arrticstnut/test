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
		if(line.empty() || (!line.empty() && line[0] == '#') || (line.size() > 1 && line[0] == '/' && line[1] == '/')) continue;
		std::istringstream iss(line);
		iss>>item>>value;	
		_confMap.insert(std::map<string,string>::value_type(item,value));
	}
	auto it = _confMap.find("StopWordsDictPath");
	if(it == _confMap.end()){
		cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:    ";
		cout<<"get stopWordDict error"<<endl;	
	}else{
		string stopWordDir = it->second;
		std::ifstream ifs(stopWordDir);
		if(!ifs.good()){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:    ";
			cout<<"open stopWordDict error"<<endl;	
		}else{//将停用词加入停用词集合
			string stopWords;
			while(ifs >> stopWords){
				if(_stopWordList.find(stopWords)== _stopWordList.end()){
					_stopWordList.insert(stopWords);
				}
			}
		}
	}
	ifs.close();
}

set<string> Configuration::getStopWordList(){
	return _stopWordList;
}
const map<string,string> & Configuration::getConfMap()const{
	return _confMap;
}

}//namespace cc
