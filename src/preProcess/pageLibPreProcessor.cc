///
/// @file    pageLibPreProcessor.cc
/// @author  
/// @date    2018-07-28 11:39:50
///

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
	WebPage::WebPage()
	{
		//processDoc(_doc,conf,jieba);
	}

	void WebPage::processDoc(const string & doc,Configuration & conf,WordSegmentation & jieba){//对格式化文档进行处理
#if 0
		//使用正则表达式提取信息
		std::regex rId("(?:<docid>)((.|\n)*)(?:</docid>)");
		std::regex rTitle("(?:<title>)((.|\n)*)(?:</title>)");
		std::regex rLink("(?:<link>)((.|\n)*)(?:</link>)");
		std::regex rCont("(?:<content>)((.|\n)*)(?:</content>)");
		std::smatch smId;
		std::smatch smTitle;
		std::smatch smLink;
		std::smatch smCont;
		if(!regex_search(doc,smId,rId) || !regex_search(doc,smTitle,rTitle) || !regex_search(doc,smLink,rLink) || !regex_search(doc,smCont,rCont)){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>\n";
			cout << "regex_search error" << endl;
			return ;
		}
		_doc = doc;
		_docId = std::stoi(smId.str(1));
		_docTitle = smTitle.str(1);
		_docUrl = smLink.str(1);
		_docContent = smCont.str(1);
#endif
		//使用正则表达式会遇到原因不明的段错误,改用string的find
		auto docidB = doc.find("<docid>");
		auto docidE = doc.find("</docid>");
		auto titleB = doc.find("<title>");
		auto titleE = doc.find("</title>");
		auto linkB = doc.find("<link>");
		auto linkE = doc.find("</link>");
		auto contentB = doc.find("<content>",linkE);
		auto contentE = doc.rfind("</content>");
		auto npos = std::string::npos;
		if(docidB == npos || docidE == npos || titleB == npos || titleE == npos 
				|| linkB == npos || linkE == npos || contentB == npos || contentE == npos){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>\n";
			cout << "search docItem error" << endl;
		}
		_doc = doc;
		docidB += string("<docid>").size();
		_docId = stoi(doc.substr(docidB,docidE - docidB));
		titleB += string("<title>").size();
		_docTitle = doc.substr(titleB,titleE - titleB);
		linkB += string("<link>").size();
		_docUrl = doc.substr(linkB,linkE - linkB);
		contentB += string("<content>").size();
		_docContent = doc.substr(contentB,contentE - contentB);
		//统计词频
		vector<string> vecWords = jieba.cutStr(_docContent);//得到分词后的单词
		set<string> stopWords = conf.getStopWordList();//得到停用词集
		//去停用词,统计词频
		_wordsMap.clear();
		for(const auto & e:vecWords){
			if(stopWords.count(e) == 0){
				_wordsMap[e] += 1;
			}
		}
		//按照词频排序：先转换成vector
		vector<pair<string,int> > vecWordFreq(_wordsMap.begin(),_wordsMap.end());
		std::sort(vecWordFreq.begin(),vecWordFreq.end(),compator());
		//得到按照词频排序的前20个单词
		_topWords.clear();
		_topWords.reserve(32);
		for(size_t i = 0;i < TOPK_NUMBER && i < vecWordFreq.size();++i){
			_topWords.push_back(vecWordFreq[i].first);
		}
	}

	int WebPage::getDocId(){
		return _docId;
	}

	string WebPage::getDoc(){
		return _doc;
	}

	map<string,int> & WebPage::getWordMap(){
		return _wordsMap;
	}

	bool operator==(const WebPage & lhs,const WebPage & rhs){//判断两篇文档是否相等
		set<string> topKWords;//topk单词集合
		size_t n = std::min(lhs._topWords.size(),rhs._topWords.size());
		n = n > 10 ? 10:n;//最多统计top10个单词的交集
		for(size_t i = 0;i < n;++i){
			string w = lhs._topWords[i];
			if(topKWords.count(w) == 0){
				topKWords.insert(w);
			}
		}
		int cnt = 0;//交集的个数
		for(size_t i = 0;i < n;++i){
			if(topKWords.count(rhs._topWords[i])){
				++cnt;
			}
		}
		double rate = (cnt * 1.0)/(n*1.0);//重复率
		return rate > 0.6;//重复率大于0.6,便认为重复了
	}

	bool operator<(const WebPage & lhs,const WebPage & rhs){//对文档按照docId进行排序
		return lhs._docId < rhs._docId;
	}


	void WebPage::calcTopK(vector<string> & wordsVec,int k,set<string> & stopWordList){//求取文档的topk词集
		if (k <= 0) return ;
		wordsVec.clear();
		wordsVec.reserve(k);
		size_t n = _topWords.size();
		size_t i = 0;
		int j = 0;
		while(i < n && j < k){
			if(stopWordList.count(_topWords[i]) == 0){
				wordsVec.push_back(_topWords[i]);
				++j;
			}
			++i;
		}
	}
	//-----------------------------------------------------------------
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
		////========================
		//for(auto & p:dfMap){
		//	cout << "word:" << p.first << " ";
		//	for(auto & dt:p.second){
		//		cout << "docId:" << dt.first <<" " << "freq:" << dt.second << ";";
		//	}
		//	cout << endl;
		//}
		////========================
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
		////========================
		//for(auto & p:normalMap){
		//	cout << "docId:" << p.first << " ";
		//	for(auto & dt:p.second){
		//		cout << "word:" << dt.first <<" " << "tfidf:" << dt.second << "; ";
		//	}
		//	cout << endl;
		//}
		//========================
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
		////========================
		//for(auto & p:normalMap){
		//	cout << "docId:" << p.first << " ";
		//	for(auto & dt:p.second){
		//		cout << "word:" << dt.first <<" " << "tfidf:" << dt.second << "; ";
		//	}
		//	cout << endl;
		//}
		////========================
#if 1
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
		//========================
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
		//========================
#endif
	}
};//end of namespace
