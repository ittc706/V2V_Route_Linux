#pragma once
#include"micro.h"
#include"object.h"

/*
* 零元函数反射注册
*/
object* new_instance(const std::string& class_name);

/*
* 零元函数反射注册
*/
void invoke(const object* obj, const std::string& method_name);

/*
* 函数反射注册
* 参数类型：int
*/
void invoke(const object* obj, const std::string& method_name, int param1);

/*
* 函数反射注册
* 参数类型：long
*/
void invoke(const object* obj, const std::string& method_name, long param1);

/*
* 函数反射注册
* 参数类型：float
*/
void invoke(const object* obj, const std::string& method_name, float param1);

/*
* 函数反射注册
* 参数类型：double
*/
void invoke(const object* obj, const std::string& method_name, double param1);

/*
* 函数反射注册
* 参数类型：void*
*/
void invoke(const object* obj, const std::string& method_name, void* param1);

/*
* 函数反射注册
* 参数类型：std::string
*/
void invoke(const object* obj, const std::string& method_name, const std::string& param1);


/*
* 函数反射注册
* 参数类型：object*
*/
void invoke(const object* obj, const std::string& method_name, object* param1);
