///
/// @file    test.cc
/// @author  
/// @date    2018-07-18 16:31:41
///

#include "configuration.h"
#include "dirScanner.h"
#include "pageLib.h"
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

int main(){
	test2();
}
