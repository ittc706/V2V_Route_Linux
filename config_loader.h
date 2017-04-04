#pragma once
#include<regex>
#include<string>
#include<fstream>
#include<map>

class config_loader {
private:
	/*
	* 配置文件字符串
	*/
	std::string m_content;

	/*
	* [标签-值]对
	*/
	std::map<std::string, std::string> m_tag_content_map;

	/*------------------方法------------------*/
public:
	/*
	* 默认构造函数
	*/
	config_loader() {}

	/*
	* 接受文件名的构造函数
	*/
	void resolv_config_file(std::string t_file);

	/*
	* 从m_tag_content_map根据标签名取出值
	*/
	std::string get_param(std::string t_param);
};