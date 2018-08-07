///
/// @file    pageLibPreProcessor.cc
/// @author  
/// @date    2018-07-28 11:39:50
///

#include "webPage.h"
#include "pageLibPreProcessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <string>
using std::cout;
using std::endl;
using std::ifstream;
using std::regex_search;
using std::log;
using std::sqrt;
using std::string;

namespace cc
{
	PageLibPreProcessor::PageLibPreProcessor(Configuration & conf,WordSegmentation & jieba)
		:_conf(conf)
		 ,_jieba(jieba)
	{
		readInfoFromFile();
		cutRedundantPages();
		buildInvertIndexTable();
	}

	void PageLibPreProcessor::readInfoFromFile(){//根据配置信息读取网页库和网页偏移库的内容
		//通过配置信息的到网页库的路径
		auto confMap = _conf.getConfMap();
		auto itlib = confMap.find("PageLibDir");
		if(itlib == confMap.end()){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>\n";
			cout << "configuration error" << endl;
		}
		string pageLibDir = itlib->second;
		//读取网页库
		ifstream ifsPageLib(pageLibDir + "pageLib.dat");
		ifstream ifsOffset(pageLibDir + "offsetLib.dat");
		if(!ifsPageLib.good() || !ifsOffset.good()){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>\n";
			cout << "ifstream error" << endl;
		}
		string line;
		long docId,startPos,len;
		while(std::getline(ifsOffset,line)){
			//将line转化成istringstream来读取配置信息
			std::istringstream iss(line);
			iss >> docId >> startPos >> len;
			//从ifs读取给定长度的字节
			ifsPageLib.seekg(startPos);//定位到指定的开头
			string doc(len,'0');//构造len长的string
			ifsPageLib.read(&doc[0],len);//读len长字符到string种
			WebPage wp;
			wp.processDoc(doc,_conf,_jieba);
			_pageLib.push_back(wp);
		}
	}

	void PageLibPreProcessor::cutRedundantPages(){//对冗余的网页进行去重
		vector<WebPage>::iterator it1 = _pageLib.begin();
		for(;it1 != _pageLib.end();){
			for(auto it2 = std::next(it1);it2 != _pageLib.end();){
				if(*it1 == *it2){
					it2 = _pageLib.erase(it2);
				}else{
					++it2;
				}
			}
			++it1;
		}
		std::sort(_pageLib.begin(),_pageLib.end());//按照docid排序
	}

	void PageLibPreProcessor::buildInvertIndexTable(){//创建倒排索引表 
		int n = _pageLib.size();//文档总数
		//以下容器参数说明:unordered_map<word,vector<pair<docid,tf> > >;
		unordered_map<string,vector<pair<int,int> > > dfMap;//用于计算df，map的key为单词
		//以下容器参数说明:unordered_map<docid,vector<pair<word,tf_idf> > >;
		unordered_map<int,vector<pair<string,double> > > normalMap;//用于每篇文档的计算每篇文档的tfidf并归一化,map的key为docid
		//统计df
		for(auto & web:_pageLib){
			map<string,int> & wordMap = web.getWordMap();
			int docId = web.getDocId();
			for(auto & p:wordMap){//p的格式pair<单词，词频>
				std::pair<int,int> dfPair = std::make_pair(docId,p.second);//dicId,词频
				//dfMap的key是单词，value是存放pair对动态数组，pair的first是文档ID，第二项是词频
				string word = p.first;
				if(dfMap.count(word) == 0){//第一次
					dfMap.insert(std::make_pair(word,vector<pair<int,int> >(1,dfPair)));
				}else{
					dfMap[word].push_back(dfPair);
				}
			}
		}

		//计算tfidf
		for(auto & p:dfMap){//dfMap的格式unordered_map<word,vector<pair<docid,tfidf> > >;
			string word = p.first;
			int df = p.second.size();//文档频率df
			for(auto & f:p.second){//f的格式:pair<docid,tfidf>
				int docId = f.first;
				double tf = f.second;
				double tf_idf = (tf * 1.0) * log((n * 1.0)/(df *1.0 + 1.0));
				vector<pair<string,double> > vecWfPair(1,make_pair(word,tf_idf));//单词，tfidf值
				if(normalMap.count(docId) == 0){//normalMap的格式:unordered_map<docid,vector<pair<word,tf_idf> > >;
					normalMap.insert(make_pair(docId,vecWfPair));
				}else{
					normalMap[docId].push_back(make_pair(word,tf_idf));
				}
			}
		}

		//归一化
		for(auto & p:normalMap){//p的格式：pair<docId,vector<word,tfidf>>
			double sum = 0;
			for(auto & wt:p.second){
				sum += (wt.second)*(wt.second);
			}
			sum = sqrt(sum);
			for(auto & wt:p.second){
				wt.second = wt.second /sum;
			}
		}

		//写入倒排索引表
		for(auto & p:normalMap){//p的格式：pair<docId,vector<word,tfidf>>
			int docId = p.first;
			for(auto & wt:p.second){
				string word = wt.first;
				double tfidf = wt.second;
				pair<int,double> dfPair = make_pair(docId,tfidf);
				if(_invertIndexTable.count(word) == 0){//_invertIndexTable的格式：unordered_map<word,vector<pair<docid,tfidf> > >

					_invertIndexTable.insert(make_pair(word,vector<pair<int,double> >(1,dfPair)));
				}else{
					_invertIndexTable[word].push_back(dfPair);
				}
			}
		}
		//将倒排索引信息写入文件
		//找到文件路径
		auto confMap = _conf.getConfMap();
		auto it  = confMap.find("PageLibDir");
		if(it == confMap.end()){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>\n";
			cout << "find pageLibDir error" << endl;
			return ;
		}
		string pageLibDir = it->second;
		//打开文件
		std::ofstream ofs(pageLibDir + "invertindex.dat");
		if(!ofs.good()){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>\n";
			cout << "open file error" << endl;
			return ;
		}
		//写入文件
		for(auto & p:_invertIndexTable){
			ofs << p.first << "\t";
			for(auto & dt:p.second){
				ofs <<  dt.first << " " <<  dt.second << "\t";
			}
			ofs << endl;
		}
		ofs.close();
	}
};//end of namespace
