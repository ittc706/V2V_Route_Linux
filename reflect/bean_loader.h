#pragma once
#include<string>
#include<vector>
#include<utility>
#include"bean_definition.h"

/*
* bean加载器
*/
class bean_loader {
private:
	/*
	* 配置文件路径
	*/
	const std::string configuration_path;

public:
	bean_loader(const std::string& path) :configuration_path(path) {}

public:
	/*
	* 加载配置文件生成bean_definition对象,并返回一个指定初始化顺序的数组,初始化顺序即下标
	*/
	std::vector<bean_definition*> load();

private:
	bean_definition* create_bean_definition(const std::string s);

	/*
	* 添加属性值
	*/
	void add_property(bean_definition* definition, const std::string s);

	/*
	* 添加依赖项
	*/
	void add_dependency(bean_definition* definition, const std::string s);

	/*
	* 添加对象构造完成后需要执行的初始化方法
	*/
	void add_pre_processor(bean_definition* definition, const std::string s);

	/*
	* 添加对象依赖注入完毕后需要执行的方法
	*/
	void add_post_processor(bean_definition* definition, const std::string s);

	/*
	* 根据依赖关系确定初始化顺序
	*/
	void order_by_dependency(std::vector<bean_definition*>& definitions);
};