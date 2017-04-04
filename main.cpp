#include<iostream>
#include"vue.h"
#include"system_control.h"
#include"context.h"

using namespace std;

int main() {
	context *__context = context::get_context();

	system_control *__system_control = __context->get_system_control();

	__system_control->process();
	
	system("pause");
}