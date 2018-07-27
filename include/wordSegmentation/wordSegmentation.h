///
/// @file    wordSegmentation.h
/// @author  
/// @date    2018-07-19 16:43:57
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

namespace cc
{
	class WordSegmentation
	{
		public:
			WordSegmentation()
			{}
			void cut(const string &);//分词
			void save(const string &);//分词后保存到文件
			vector<string> getCutWords();//得到分词后的结果
			void setPath(const string &,const string &,const string &,const string &,const string &);
			void setDictPath(const string &);
			void setHmmPath(const string &);
			void setUserDictPath(const string &);
			void setIdfPath(const string &);
			void setStopWordPath(const string &);
		private:
			//路径是相对于 Jieba.hpp 的
			//const char* const DICT_PATH = "../data/cppjiebaDict/dict/jieba.dict.utf8";
			//const char* const HMM_PATH = "../data/cppjiebaDict/dict/hmm_model.utf8";
			//const char* const USER_DICT_PATH = "../data/cppjiebaDict/dict/user.dict.utf8";
			//const char* const IDF_PATH = "../data/cppjiebaDict/dict/idf.utf8";
			//const char* const STOP_WORD_PATH = "../data/cppjiebaDict/dict/stop_words.utf8";
			
			//路径是相对于 Jieba.hpp 的
			string _dict_path = "./cppjiebaDict/dict/jieba.dict.utf8";//jieba词典路径
			string _hmm_path = "./cppjiebaDict/dict/hmm_model.utf8";//hmm模型路径
			string _user_dict_path = "./cppjiebaDict/dict/user.dict.utf8";//用户自定义词典路径
			string _idf_path = "./cppjiebaDict/dict/idf.utf8";;//倒排词典路径
			string _stop_word_path = "./cppjiebaDict/dict/stop_words.utf8";//停用词词典路径
		private:
			vector<string> _vecWords;//分词后的结果
	};

}//namespace cc

