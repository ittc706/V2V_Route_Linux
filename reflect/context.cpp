#include"context.h"
#include"invoke.h"
#include<iostream>

using namespace std;

context* context::singleton = nullptr;

context::context(const std::string& path) :configuration_path(path) {
	loader = new bean_loader(path);
}

context::~context() {
	delete loader;
}

void context::create_context(const std::string& path) {
	static context _context = context(path);
	singleton = &_context;
	singleton->init();
}

context* context::get_context() {
	assert(singleton != nullptr);
	return singleton;
}

void context::init() {
	parse_configuration();

	create_and_init_bean();

	pre_process();

	dependency_injection();

	post_process();
}

void context::parse_configuration() {
	bean_definitions.swap(loader->load());
}

void context::create_and_init_bean() {
	for (bean_definition* definition : bean_definitions) {
		string bean_id = definition->id;
		string class_type = definition->class_type;
		object* _obj = new_instance(class_type);
		if (_obj == nullptr) {
			cout << "类型 <" << class_type << ">尚未注册工厂方法，请于 object* new_instance(const std::string& class_name) 方法中完成注册" << endl;
			system("pause");
			exit(0);
		}
		assert(bean_map.insert({ bean_id ,_obj }).second);
		for (bean_property p : definition->properties) {
			invoke(_obj, "set_" + p.name, p.value);
		}
	}
}

void context::dependency_injection() {
	for (bean_definition* definition : bean_definitions) {
		string bean_id = definition->id;
		assert(bean_map.find(bean_id) != bean_map.end());
		for (bean_dependency dependency : definition->dependencies) {
			string dependency_bean_id = dependency.ref_id;
			assert(bean_map.find(dependency_bean_id) != bean_map.end());
			invoke(bean_map[bean_id], "set_" + dependency.name, bean_map[dependency_bean_id]);
		}
	}
}

void context::pre_process() {
	for (bean_definition* definition : bean_definitions) {
		string bean_id = definition->id;
		object* __obj = bean_map[bean_id];
		for (string pre_processor : definition->pre_processors) {
			invoke(__obj, pre_processor);
		}
	}
}

void context::post_process() {
	for (bean_definition* definition : bean_definitions) {
		string bean_id = definition->id;
		object* __obj = bean_map[bean_id];
		for (string post_processor : definition->post_processors) {
			invoke(__obj, post_processor);
		}
	}
}

object* context::get_bean(std::string bean_id) {
	assert(bean_map.find(bean_id) != bean_map.end());
	return bean_map[bean_id];
}
