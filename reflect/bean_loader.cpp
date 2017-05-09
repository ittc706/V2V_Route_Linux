#include<regex>
#include<fstream>
#include<iostream>
#include<queue>
#include<set>
#include<map>
#include<utility>
#include<assert.h>
#include"bean_loader.h"

using namespace std;

vector<bean_definition*> bean_loader::load() {
	vector<bean_definition*> definitions;

	const regex BEAN_REGEX("<bean([^>]+)>([\\s\\S]*?)</bean>");

	const regex ANNOTATION_REGEX("<!--[\\s\\S]*?-->");


	//读取待解析字符串
	ifstream in(configuration_path);
	if (!in.is_open()) {
		cout << "配置文件路径有误：" << configuration_path << endl;
		system("pause");
		exit(0);
	}
	istreambuf_iterator<char> if_it(in), if_eof;
	string content(if_it, if_eof);
	in.close();


	//首先除去注释
	content = regex_replace(content, ANNOTATION_REGEX, "");


	//解析，并存储
	for (sregex_iterator it(content.begin(), content.end(), BEAN_REGEX), eof; it != eof; ++it) {
		string inside_info = it->operator[](1);
		string content_info = it->operator[](2);

		bean_definition* definition = create_bean_definition(inside_info);

		add_property(definition, content_info);
		add_dependency(definition, content_info);
		add_pre_processor(definition, content_info);
		add_post_processor(definition, content_info);

		definitions.push_back(definition);
	}

	//根据依赖关系确定有向图访问顺序
	order_by_dependency(definitions);

	return definitions;
}

bean_definition* bean_loader::create_bean_definition(const std::string s) {
	bean_definition* definition = new bean_definition();
	const regex REGEX("id *= *\"([^\"]+)\" *, *class *= *\"([^\"]+)\"");
	sregex_iterator it(s.begin(), s.end(), REGEX);
	definition->id = it->operator[](1);
	definition->class_type = it->operator[](2);
	return definition;
}

void bean_loader::add_property(bean_definition* definition, const std::string s) {
	const regex REGEX_NOT_BEAN("<property +name *= *\"([^\"]+)\" *, *value *= *\"([^\"]+)\" */>");
	for (sregex_iterator it(s.begin(), s.end(), REGEX_NOT_BEAN), eof; it != eof; ++it) {
		definition->properties.push_back(bean_property(
			it->operator[](1),
			it->operator[](2),
			false
			));
	}

	const regex REGEX_BEAN("<property +name *= *\"([^\"]+)\" *, *ref-id *= *\"([^\"]+)\" */>");
	for (sregex_iterator it(s.begin(), s.end(), REGEX_BEAN), eof; it != eof; ++it) {
		definition->properties.push_back(bean_property(
			it->operator[](1),
			it->operator[](2),
			true
			));
	}
}

void bean_loader::add_dependency(bean_definition* definition, const std::string s) {
	const regex REGEX("<dependency +ref-id *= *\"([^\"]+)\" */>");
	for (sregex_iterator it(s.begin(), s.end(), REGEX), eof; it != eof; ++it) {
		definition->dependencies.push_back(bean_dependency(
			it->operator[](1)
			));
	}
}

void bean_loader::add_pre_processor(bean_definition* definition, const std::string s) {
	const regex REGEX("<pre-processor +method_name *= *\"([^\"]+)\" */>");
	for (sregex_iterator it(s.begin(), s.end(), REGEX), eof; it != eof; ++it) {
		definition->pre_processors.push_back(it->operator[](1));
	}
}

void bean_loader::add_post_processor(bean_definition* definition, const std::string s) {
	const regex REGEX("<post-processor +method_name *= *\"([^\"]+)\" */>");
	for (sregex_iterator it(s.begin(), s.end(), REGEX), eof; it != eof; ++it) {
		definition->post_processors.push_back(it->operator[](1));
	}
}


void bean_loader::order_by_dependency(std::vector<bean_definition*>& definitions) {
	map<string, bean_definition*> bean_map;
	map<string, vector<string>> adj_map;
	map<string, int> degree_map;
	for (bean_definition* definition : definitions) {
		string bean_id = definition->id;
		if (bean_map.find(bean_id) != bean_map.end()) {
			cout << "bean id <" << bean_id << ">重复，请保证配置文件中所有bean id的唯一性" << endl;
			system("pause");
			exit(0);
		}
		bean_map.insert({ bean_id,definition });
		for (bean_dependency dependency : definition->dependencies) {
			string dependency_bean_id = dependency.ref_id;
			if (adj_map.find(dependency_bean_id) == adj_map.end()) {
				adj_map.insert({ dependency_bean_id,{ bean_id } });
			}
			else {
				assert(adj_map[dependency_bean_id].size() > 0);
				adj_map[dependency_bean_id].push_back(bean_id);
			}
		}
		assert(degree_map.insert({ bean_id,definition->dependencies.size() }).second);
	}
	queue<string> queue;
	int cnt = 0;
	for (pair<string, int> key_value: degree_map){
		if (key_value.second == 0) {
			queue.push(key_value.first);
			cnt++;
		}
	}
	vector<bean_definition*> temp;
	while (!queue.empty()) {
		string bean_id = queue.front();
		queue.pop();

		bean_definition* definition = bean_map[bean_id];
		temp.push_back(definition);
		const vector<string>& adj_list = adj_map[bean_id];
		for (string other_bean_id : adj_list) {
			if (--degree_map[other_bean_id] == 0) {
				queue.push(other_bean_id);
				cnt++;
			}
		}
	}
	if (cnt != definitions.size()) {
		cout << "当前依赖关系导致有向图无法遍历，请重新确定bean依赖关系" << endl;
		system("pause");
		exit(0);
	}
	definitions.swap(temp);
}
