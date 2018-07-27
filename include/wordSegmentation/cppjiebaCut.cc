 ///
 /// @file    cppjiebaCut.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2017-07-28 11:42:40
 ///
#include "Jieba.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using namespace cppjieba;
//const char* const DICT_PATH = "./dict/jieba.dict.utf8";
//const char* const HMM_PATH = "./dict/hmm_model.utf8";
//const char* const USER_DICT_PATH = "./dict/user.dict.utf8";
//const char* const IDF_PATH = "./dict/idf.utf8";
//const char* const STOP_WORD_PATH = "./dict/stop_words.utf8";

//以下路径是相对于 Jieba.hpp 的
const char* const DICT_PATH = "../data/cppjiebaDict/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../data/cppjiebaDict/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../data/cppjiebaDict/dict/user.dict.utf8";
const char* const IDF_PATH = "../data/cppjiebaDict/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../data/cppjiebaDict/dict/stop_words.utf8";
namespace cc
{
void cppjiebaCut(const string & fileCnPath,const string & fileCnPathCut) {
	cppjieba::Jieba jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH,
        IDF_PATH,
        STOP_WORD_PATH);
	vector<string> res;
	string doc;
	ifstream ifs(fileCnPath);
	ofstream ofs(fileCnPathCut); 
	if(!ifs.good() || !ofs.good()){
		cout<<"open fileCnPath error"<<endl;	
		ifs.close();
		ofs.close();
		return;
	}
	while(ifs>>doc){
		res.clear();
	    jieba.Cut(doc, res);
		for(auto & elem:res){
			ofs<<elem<<" ";	
		}
		ofs<<endl;
	}
	ifs.close();
	ofs.close();
}

}//namespace cc
