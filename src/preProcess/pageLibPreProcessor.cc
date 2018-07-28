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
using std::cout;
using std::endl;
using std::ifstream;

namespace cc
{
		WebPage::WebPage(string & doc,Configuration & conf,WordSegmentation & jieba)
		:_doc(doc)
		,_conf(conf)
		,_jieba(jieba)
		{
			processDoc(_doc,_conf,_jieba);
		}

		void WebPage::processDoc(const string & doc,Configuration & conf,WordSegmentation & jieba){//对格式化文档进行处理
			std::regex rDocId("<title>.*</title>")
			//pass
		}

		void WebPage::calcTopK(vector<string> & wordsVec,int k,set<string> & stopWordList){//求取文档的topk词集
			//pass
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
				_pageLib.push_back(WebPage(doc,_conf,_jieba));
			}
		}

};//end of namespace
