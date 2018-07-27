///
/// @file    xmlparse.cc
/// @author  lemon(haohb13@gmail.com)
/// @date    2017-06-16 19:27:29
///
#include "tinyxml2.h" 
#include "xmlprase.h"
#include  <regex>

using namespace tinyxml2;
namespace cc
{
	RssReader::RssReader() {
		_rss.reserve(128);
	}

	void RssReader::parseRss(const string & filename){//解析rss
		XMLDocument doc;
		if(doc.LoadFile(filename.c_str())!=tinyxml2::XMLError::XML_SUCCESS){
			cout<<"[LoadFile]:[error:id="<<doc.ErrorID()<<"]:["<<filename<<"]"<<endl;
			return;
		}else{
			//cout<<"[LoadFile]:[success]:"<<"["<<filename<<"]"<<endl;
		}
		XMLElement *root=doc.RootElement();
		string rootname=root->Name();
		string itemname="item";
		string titlename="title";
		string linkname="link";
		string contentname="description";
		XMLElement *item;
		if(rootname=="feed"){//处理根为feed的xml
			itemname="entry";
			contentname="content";
			item=root->FirstChildElement(itemname.c_str());
		}else{
			XMLElement *channel=root->FirstChildElement();
			item=channel->FirstChildElement(itemname.c_str());
		}
		int itemcnt=0;//记录是否处理了数据
		std::regex reg("<{1}[^<>]*>{1}");
		while(item){
			struct RssItem rssitem;
			XMLElement *title=item->FirstChildElement(titlename.c_str());
			if(title->GetText()==NULL){
				item=item->NextSiblingElement(itemname.c_str());
				continue;
			}
			rssitem.title=title->GetText();
			rssitem.title=regex_replace(rssitem.title,reg,"");
			XMLElement *link=item->FirstChildElement(linkname.c_str());
			if(link->GetText()==NULL){//处理根为feed类型xml的link
				const XMLAttribute *linkAttr=link->FirstAttribute();
				rssitem.link=linkAttr->Value();
				rssitem.link=regex_replace(rssitem.link,reg,"");
			}else{
				rssitem.link=link->GetText();
				rssitem.link=regex_replace(rssitem.link,reg,"");
			}
			XMLElement *content=item->FirstChildElement(contentname.c_str());
			rssitem.content=content->GetText();
			rssitem.content=regex_replace(rssitem.content,reg,"");
			_rss.push_back(rssitem);
			item=item->NextSiblingElement(itemname.c_str());
			++itemcnt;
		}
		//if(itemcnt==0){
		//	cout<<"[Parse]:[error:no datas]:["<<filename<<"]"<<endl;
		//}else{
		//	cout<<"[Parse]:[success]:["<<filename<<"]"<<endl;
		//}		
	}


	void RssReader::dump(const string & resultfile){
		_rss.shrink_to_fit();
		std::ofstream ofs(resultfile,std::ios::app);
		if(!ofs.good()){
			cout<<"ofstream open error"<<endl;
			return;
		}
		int idx=0;
		int size=_rss.size();
		for(idx=0;idx < size;++idx){
			ofs<<"<doc>"<<endl;
			ofs<<'\t'<<"<title>"<<_rss[idx].title<<"</title>"<<endl;
			ofs<<'\t'<<"<link>"<<_rss[idx].link<<"</link>"<<endl;
			ofs<<'\t'<<"<content>"<<_rss[idx].content<<"</content>"<<endl;
			ofs<<"</doc>"<<endl;
		}
		ofs.close();
	}

	PageDataReader::PageDataReader()
	{
		_vecPageItem.reserve(128);
	}


	void PageDataReader::parsePageData(const string & filename){//解析从网页爬取下来的网页数据
		XMLDocument docs;//docs就是整个网页库
		if(docs.LoadFile(filename.c_str())!=tinyxml2::XMLError::XML_SUCCESS){
			cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>>>\n"; 
			cout<<"open file \""<< filename << " \"error,erron = "<<docs.ErrorID()<<endl;
			return;
		}else{
			//cout<<"[LoadFile]:[success]:"<<"["<<filename<<"]"<<endl;
		}
		XMLElement *doc = docs.RootElement();//得到第1个doc
		XMLElement *title,*link,*content;
		//XMLElement *description;
		PageItem pageItem;
		while(doc){
			title = doc->FirstChildElement("title");
			link = doc->FirstChildElement("link");
			content = doc->FirstChildElement("content");
			//description = doc->FirstChildElement("description");
			if(!title || !link || !content){
				cout<<"@["<<__FILE__<<"::"<<__FUNCTION__<<"]:>>\n";
				cout<< "pagelib attribute name error" << endl;
			}
			const char *pTitleText = title->GetText();
			const char *pLinkText = link->GetText();
			const char *pContentText = content->GetText();
			//要判断指针是否为空，否则赋值给string会出错
			pageItem.title = pTitleText ? pTitleText : "";
			pageItem.link =  pLinkText ? pLinkText : "";
			pageItem.content = pContentText ? pContentText : "";
			_vecPageItem.push_back(pageItem);
			doc = doc->NextSiblingElement();//继续下一个doc
		}
	}


	vector<PageItem> PageDataReader::getPageItems(){
		return _vecPageItem;
	}

	void PageDataReader::dump(const string & outfileName){
		_vecPageItem.shrink_to_fit();
		std::ofstream ofs(outfileName,std::ios::app);
		if(!ofs.good()){
			cout<<"ofstream open error"<<endl;
			return;
		}
		int idx=0;
		int size=_vecPageItem.size();
		for(idx=0;idx < size;++idx){
			ofs<<"<doc>"<<endl;
			ofs<<'\t'<<"<title>"<<_vecPageItem[idx].title<<"</title>"<<endl;
			ofs<<'\t'<<"<link>"<<_vecPageItem[idx].link<<"</link>"<<endl;
			ofs<<'\t'<<"<content>"<<_vecPageItem[idx].content<<"</content>"<<endl;
			ofs<<"</doc>"<<endl;
		}
		ofs.close();
	}

};//end of namespace



