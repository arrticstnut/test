 ///
 /// @file    configuration.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2017-07-18 11:08:54
 ///
#ifndef __CC_CONFIGURATION_H__
#define __CC_CONFIGURATION_H__
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using std::map;
using std::string;
using std::cout;
using std::endl;
namespace cc
{
class Configuration
{
	public:
		Configuration(const string &);
		map<string,string> & getConfMap();
	private:
		string _confFilePath;
		map<string,string> _confMap;	
};
}//namespace cc
#endif
