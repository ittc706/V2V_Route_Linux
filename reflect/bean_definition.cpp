#include<iostream>
#include<sstream>
#include"bean_definition.h"

using namespace std;

std::string bean_property::to_string(int n) {
	string space = "";
	for (int i = 0; i < n; i++) {
		space = space + " ";
	}
	stringstream ss;
	ss << space << "[" << endl;
	ss << space << "    name: " << name << endl;
	ss << space << "    value: " << value << endl;
	ss << space << "]," << endl;
	return ss.str();
}

std::string bean_dependency::to_string(int n) {
	string space = "";
	for (int i = 0; i < n; i++) {
		space = space + " ";
	}
	stringstream ss;
	ss << space << "[" << endl;
	ss << space << "    ref-id: " << ref_id << endl;
	ss << space << "]," << endl;
	return ss.str();
}

std::string bean_definition::to_string() {
	stringstream ss;
	ss << "id: " << id << endl;
	ss << "class_type: " << class_type << endl;
	ss << "properties: " << endl;
	ss << "    {" << endl;
	for (bean_property property : properties) {
		ss << property.to_string(8) << endl;
	}
	ss << "    }" << endl;
	ss << "dependencies: " << endl;
	ss << "    {" << endl;
	for (bean_dependency dependency : dependencies) {
		ss << dependency.to_string(8) << endl;
	}
	ss << "    }" << endl;
	ss << "pre-processor: " << endl;
	ss << "    {" << endl;
	for (string pre_processor : pre_processors) {
		ss << "        " << pre_processor << endl;
	}
	ss << "    }" << endl;
	ss << "post-processor: " << endl;
	ss << "    {" << endl;
	for (string post_processor : post_processors) {
		ss << "        " << post_processor << endl;
	}
	ss << "    }" << endl;
	return ss.str();
}