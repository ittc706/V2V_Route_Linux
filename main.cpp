#include<iostream>
#include<fstream>
#include"system_control.h"
#include"reflect/context.h"
using namespace std;


int main() {
	context::create_context("config/beans.xml");
	context *__context = context::get_context();

	system_control *__system = (system_control *)__context->get_bean("system_control");

	__system->process();
	system("pause");
}