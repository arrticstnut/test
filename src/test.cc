///
/// @file    test.cc
/// @author  
/// @date    2018-07-18 16:31:41
///

#include "configuration.h"
//#include "wordSegmentation.h"
#include "dirScanner.h"
#include "pageLib.h"
#include "pageLibPreProcessor.h"
#include <fstream>
#include <sstream>
#include <iostream>
using std::cout;
using std::endl;

int test1(){
	cc::Configuration conf("../configuration/configuration.conf");
	cc::DirScanner ds;
	auto ret = ds.files();
	for(const auto & e:ret){
		cout << e << endl;
	}
	ret = ds("/home/ubuntu/prt/cpp/cppSer/rssSearchEngine/server");
	for(const auto & e:ret){
		cout << e << endl;
	}
	return 0;
}

void test2(){
	cc::Configuration conf("../configuration/configuration.conf");//相对于bin运行时所在的路径
	cc::DirScanner dirScanner;
	cc::PageLib pageLib(conf,dirScanner);
	cout << "1" << endl;
	pageLib.create();
	cout << "2" << endl;
	pageLib.store();
}


void test3(){

	//读取网页库
	std::ifstream ifsPageLib("../data/pageLib/pageLib.dat");
	std::ifstream ifsOffset("../data/pageLib/offsetLib.dat");
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
		//===========================
		cout << "======================";
		cout<< doc;
		cout << "======================";
		//===========================
	}
}

void test4(){
	string doc = "<doc>\n <docid>233</docid>\n <title>欢迎加入微信：一些准备工作</title>\n <link>https://xbeta.info/wechat.htm</link>\n <content>因工作繁忙，本博客更新日益减少。\n 微博更新相对多一些：@善用佳软  http://weibo.com/xbeta\n 微信公众号（扫码添加）偶尔会写一些内容。\n 关注微信公众号之后，也可以按操作找朋友拉入微信群：xbeta微信试验群 ——如何使用，还没想好  。加入后保先持沉默即可，不用打招呼。反正我不太聊天。\n ......(共147字)</content>\n </doc>";
	cc::Configuration conf("../configuration/configuration.conf");//相对于bin运行时所在的路径
	cc::WordSegmentation jieba;
	cc::WebPage webPage;
	webPage.processDoc(doc,conf,jieba);
}

void test6(){
	cc::Configuration conf("../configuration/configuration.conf");//相对于bin运行时所在的路径
	cc::WordSegmentation jieba;
	cc::PageLibPreProcessor preProcessor(conf,jieba);
}
int main(){
	test6();
}
