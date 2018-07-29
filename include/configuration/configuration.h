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
#include <vector>
#include <set>
using std::map;
using std::string;
using std::cout;
using std::endl;
using std::set;
using std::vector;
namespace cc
{
class Configuration
{
	public:
		Configuration(const string &);
		const map<string,string> & getConfMap()const;
		set<string> getStopWordList();
	private:
		string _confFilePath;
		map<string,string> _confMap;
		set<string> _stopWordList;//停用词
};
}//namespace cc
#endif
