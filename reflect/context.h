#pragma once
#include<unordered_map>
#include<string>
#include<assert.h>
#include"object.h"
#include"bean_loader.h"
#include"bean_definition.h"

/*
* 容器类
*/
class context {
private:
	/*
	* 配置文件路径
	*/
	const std::string configuration_path;

	/*
	* bean映射字典
	*/
	std::unordered_map<std::string, object*> bean_map;

	/*
	* 配置文件加载器
	*/
	bean_loader* loader = nullptr;

	std::vector<bean_definition*> bean_definitions;

	/*
	* 单例模式,私有化构造函数
	*/
private:
	static context* singleton;
private:
	context(const std::string& path);
public:
	~context();
	static void create_context(const std::string& path);
	static context* get_context();

private:
	/*
	* 容器初始化
	*/
	void init();

	/*
	* 解析配置文件
	*/
	void parse_configuration();

	/*
	* 按照指定顺序初始化所有bean
	*/
	void init_bean();

	/*
	* 属性注入，包括一般属性以及bean属性
	*/
	void property_injection();

	/*
	* 预处理器
	*/
	void pre_process();

	/*
	* 后处理器
	*/
	void post_process();
public:
	object* get_bean(std::string bean_id);
};