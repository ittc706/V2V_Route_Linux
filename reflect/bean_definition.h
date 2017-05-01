#pragma once
#include<string>
#include<vector>

struct bean_property {
	const std::string name;//属性名
	const std::string value;//属性值,保存为string
	bean_property(const std::string& t_name, const std::string& t_value)
		:name(t_name), value(t_value) {}

	std::string to_string(int n);
};

struct bean_dependency {
	const std::string name;//属性名
	const std::string ref_id;//依赖的bean id
	bean_dependency(const std::string& t_name, const std::string& t_ref_id)
		:name(t_name), ref_id(t_ref_id) {}

	std::string to_string(int n);
};

struct bean_definition {
	std::string id;//bean的唯一id
	std::string class_type;//bean的类型

	std::vector<bean_property> properties;//属性集合
	std::vector<bean_dependency> dependencies;//依赖项
	std::vector<std::string> pre_processors;//预处理器(对象构造之后,依赖注入之前需要执行的初始化方法)
	std::vector<std::string> post_processors;//后处理器(依赖注入后需要执行的初始化方法)
	std::string to_string();
};