#include<stdexcept>
#include"micro.h"
#include"object.h"



/*
* 零元函数反射注册
*/
object* new_instance(const std::string& class_name) {
	FACTORY_INVOKE_HEAD

		return nullptr;
}

/*
* 零元函数反射注册
*/
void invoke(const object* obj, const std::string& method_name) {
	long class_id = obj->get_class_id();

}

/*
* 函数反射注册
* 参数类型：int
*/
void invoke(const object* obj, const std::string& method_name, int param1) {
	long class_id = obj->get_class_id();

}

/*
* 函数反射注册
* 参数类型：long
*/
void invoke(const object* obj, const std::string& method_name, long param1) {
	long class_id = obj->get_class_id();


}

/*
* 函数反射注册
* 参数类型：float
*/
void invoke(const object* obj, const std::string& method_name, float param1) {
	long class_id = obj->get_class_id();


}

/*
* 函数反射注册
* 参数类型：double
*/
void invoke(const object* obj, const std::string& method_name, double param1) {
	long class_id = obj->get_class_id();

}

/*
* 函数反射注册
* 参数类型：void*
*/
void invoke(const object* obj, const std::string& method_name, void* param1) {
	long class_id = obj->get_class_id();

}

/*
* 函数反射注册
* 参数类型：void*
*/
void invoke(const object* obj, const std::string& method_name, const std::string& param1) {
	long class_id = obj->get_class_id();
	METHOD_INVOKE_CLASS_START
}
