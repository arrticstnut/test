///
/// @file    wordSegmentation.cc
/// @author  
/// @date    2018-07-27 10:07:29
///

#include "wordSegmentation.h"
#include <string>
#include <iostream>
using std::cout;
using std::endl;

namespace cc
{

	
	//路径是相对于 .exe 的
	WordSegmentation::WordSegmentation()
	{
			_dict_path = "../include/wordSegmentation/cppjiebaDict/dict/jieba.dict.utf8";
			_hmm_path = "../include/wordSegmentation/cppjiebaDict/dict/hmm_model.utf8";
			_user_dict_path = "../include/wordSegmentation/cppjiebaDict/dict/user.dict.utf8";
			_idf_path = "../include/wordSegmentation/cppjiebaDict/dict/idf.utf8";
			_stop_word_path = "../include/wordSegmentation/cppjiebaDict/dict/stop_words.utf8";
	}
	void WordSegmentation::save(const string & fileOut) {
		ofstream ofs(fileOut);
		if(!ofs.good()){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>\n"; 
			cout<<"open file error"<<endl;
			ofs.close();
			return;
		}
		for(auto & elem:_vecWords){
			ofs<<elem<<" ";
		}
		ofs.close();
	}

	vector<string> WordSegmentation::cutFile(const string & fileIn) {
		cppjieba::Jieba jieba(
				_dict_path.c_str(),
				_hmm_path.c_str(),
				_user_dict_path.c_str(),
				_idf_path.c_str(),
				_stop_word_path.c_str());
		vector<string> res;
		string doc;
		ifstream ifs(fileIn);
		if(!ifs.good()){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>\n"; 
			cout<<"open file error"<<endl;
			ifs.close();
			return _vecWords;
		}
		while(ifs>>doc){
			res.clear();
			jieba.Cut(doc, res);
			for(auto & elem:res){
				_vecWords.push_back(elem);
			}
		}
		ifs.close();
		return _vecWords;
	}

	vector<string> WordSegmentation::cutStr(const string & str) {
		cppjieba::Jieba jieba(
				_dict_path.c_str(),
				_hmm_path.c_str(),
				_user_dict_path.c_str(),
				_idf_path.c_str(),
				_stop_word_path.c_str());
		vector<string> res;
		string doc;
		istringstream iss(str);
		if(!iss.good()){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>\n"; 
			cout<<"open str error"<<endl;
			return _vecWords;
		}
		while(iss>>doc){
			res.clear();
			jieba.Cut(doc, res);
			for(auto & elem:res){
				_vecWords.push_back(elem);
			}
		}
		return _vecWords;
	}

	vector<string> WordSegmentation::getCutWords(){
		return _vecWords;
	}

	void WordSegmentation::setDictPath(const string & dict_path){
		_dict_path = dict_path;
	}

	void WordSegmentation::setHmmPath(const string & hmm_path){
		_hmm_path = hmm_path;
	}

	void WordSegmentation::setUserDictPath(const string & user_dict_path){
		_user_dict_path = user_dict_path;
	}

	void WordSegmentation::setIdfPath(const string & idf_path){
		_idf_path = idf_path;
	}

	void WordSegmentation::setStopWordPath(const string & stop_word_path){
		_stop_word_path = stop_word_path;
	}

	void WordSegmentation::setPath(
			const string & dict_path,
			const string & hmm_path,
			const string & user_dict_path,
			const string & idf_path,
			const string & stop_word_path){
		setDictPath(dict_path);
		setHmmPath(hmm_path);
		setUserDictPath(user_dict_path);
		setIdfPath(idf_path);
		setStopWordPath(stop_word_path);
	}
};
