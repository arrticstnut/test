///
/// @file    test.cc
/// @author  
/// @date    2018-07-18 16:31:41
///

#include "configuration.h"
#include "dirScanner.h"
#include "pageLib.h"
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
		cout<< doc;
		cout << "======================";
	}
}

int main(){
	test3();
}
