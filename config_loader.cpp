/*
* =====================================================================================
*
*       Filename:  ConfigLoader.cpp
*
*    Description:  配置文件加载器
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  HCF
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<iostream>
#include<sstream>
#include<stdexcept>
#include"config_loader.h"

using namespace std;

void config_loader::resolv_config_file(string t_file) {
	//清空缓存
	m_content.clear();
	m_tag_content_map.clear();

	//读取待解析字符串
	ifstream in(t_file);
	istreambuf_iterator<char> if_it(in), if_eof;
	m_content.assign(if_it, if_eof);
	in.close();

	//解析，并存储
	regex r("<([^<>]*)>([^<>]*)</([^<>]*)>");
	for (sregex_iterator it(m_content.begin(), m_content.end(), r), eof; it != eof; ++it) {
		string leftTag = it->operator[](1);
		string rightTag = it->operator[](3);
		string content = it->operator[](2);
		if (leftTag != rightTag) {
			throw logic_error("tag not match");
		}
		m_tag_content_map[leftTag] = content;
	}
}

std::string config_loader::get_param(std::string t_param) {
	if (m_tag_content_map.find(t_param) == m_tag_content_map.end()) return "";
	return m_tag_content_map[t_param];
}