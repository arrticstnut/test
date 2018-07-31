///
/// @file    pageLibPreProcessor.h
/// @author  
/// @date    2018-07-28 10:58:42
///
#ifndef __CC_PAGELIBPREPROCESSOR_H__
#define __CC_PAGELIBPREPROCESSOR_H__
#include "configuration.h"
#include "wordSegmentation.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;
using std::map;
using std::set;
namespace cc
{

	class WebPage
	{
		public:
			WebPage();
		public:
			int getDocId();//获取文档id
			string getDoc();//获取文档
			map<string,int> & getWordMap();//获取文档的词频统计map
			void processDoc(const string & doc,Configuration & conf,WordSegmentation & jieba);//对格式化文档进行处理
			void calcTopK(vector<string> & wordsVec,int k,set<string> & stopWordList);//求取文档的topk词集
		private:
			const static int TOPK_NUMBER = 20;
			string _doc;//整篇文档
			int _docId;//文档id
			string _docTitle;//文档标题
			string _docUrl;//文档Url
			string _docContent;//文档内容
			string _docSummary;//文档摘要，自动生成，不固定
			vector<string> _topWords;//词频最高的前20个词
			map<string,int> _wordsMap;//保存每篇文档的所有词语和词频，不包括停用词
		public:
			friend bool operator==(const WebPage & lhs,const WebPage & rhs);//判断两篇文档是否相等
			friend bool operator<(const WebPage & lhs,const WebPage & rhs);//对文档按照docId进行排序
		public:
			class compator{//按照词频排序的比较对象
				public:
					bool operator()(const pair<string,int> & lhs,const pair<string,int> & rhs){
						if(lhs.second != rhs.second){
							return lhs.second > rhs.second;
						}
						return lhs.first < rhs.first;
					}
			};
	};

	class PageLibPreProcessor
	{
		public:
			PageLibPreProcessor(Configuration & conf,WordSegmentation & jieba);
			void doPreProcess();//执行预处理
			void readInfoFromFile();//根据配置信息读取网页库和网页偏移库
			void cutRedundantPages();//对冗余的网页进行去重
			void buildInvertIndexTable();//创建倒排索引表
			void storeOnDisk();//将经过预处理后的网页库、位置偏移库和倒排索引表写回到磁盘上
		private:
			Configuration & _conf;
			WordSegmentation & _jieba;//分词对象
			vector<WebPage> _pageLib;//网页库的容器对象
			unordered_map<int,pair<int,int> > _offsetLib;//网页偏移库对象
			unordered_map<string,vector<pair<int,double> > > _invertIndexTable;//倒排索引表对象
	};



};//end of namespace



#endif
