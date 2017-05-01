#pragma once
#include<iostream>

/*----------------------------------------------------------------------------
* >>>>>>>>>>>>>>>>>>>>>>>>删除拷贝控制成员宏(START)<<<<<<<<<<<<<<<<<<<<<<<<<<<
* ---------------------------------------------------------------------------*/
/*
* 将拷贝构造函数、移动构造函数、拷贝赋值运算符、移动赋值运算符定义为删除
*/
#define DELETE_COPY_MEMBER(class_type) \
public:\
class_type(){}\
class_type(const class_type&) = delete;\
class_type(class_type&&) = delete;\
class_type& operator=(const class_type&) = delete;\
class_type& operator=(class_type&&) = delete;\


/*----------------------------------------------------------------------------
* >>>>>>>>>>>>>>>>>>>>>>>>>删除拷贝控制成员宏(END)<<<<<<<<<<<<<<<<<<<<<<<<<<<<
* ---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
* >>>>>>>>>>>>>>>>>>>>>>>>>>>>bean类注册宏(START)<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
* ---------------------------------------------------------------------------*/

/*
* 友元声明
*/
#define INVOKE_FRIEND_DECLARED \
friend void invoke(const object* obj, const std::string& method_name);\
\
friend void invoke(const object* obj, const std::string& method_name, int param1); \
\
friend void invoke(const object* obj, const std::string& method_name, long param1); \
\
friend void invoke(const object* obj, const std::string& method_name, float param1); \
\
friend void invoke(const object* obj, const std::string& method_name, double param1); \
\
friend void invoke(const object* obj, const std::string& method_name, void* param1); \
\
friend void invoke(const object* obj, const std::string& method_name, const std::string& param1); \
\
friend void invoke(const object* obj, const std::string& method_name, object* param1); \


/*
* 注册类型id的宏
*/
#define REGISTE_CLASS_ID_HEAD(class_type)\
public:\
	static const long class_id;\

/*
* 定义工厂方法
*/
#define REGISTE_FACTORY_METHOD(class_type)\
public:\
	static object* new_instance(){\
		return new class_type();\
	}\
private:

/*
* 重载operator new操作符的宏
*/
#define OVERRIDE_OPERATOR_NEW(class_type)\
public:\
	void* operator new(size_t size) {\
		return object::operator new(size, class_type::class_id);\
	}\
private:

/*
* 重载operator delete操作符的宏
*/
#define OVERRIDE_OPERATOR_DELETE(class_type)\
public:\
	void operator delete(void*p){\
		object::operator delete(p);\
	}\
private:

/*
* 该宏组合了上面几个宏，用于被反射的类进行注册
*/
#define REGISTE_MEMBER_HEAD(class_type)\
DELETE_COPY_MEMBER(class_type) \
INVOKE_FRIEND_DECLARED \
REGISTE_CLASS_ID_HEAD(class_type) \
REGISTE_FACTORY_METHOD(class_type) \
OVERRIDE_OPERATOR_NEW(class_type) \
OVERRIDE_OPERATOR_DELETE(class_type) \

/*
* 在源文件中初始化类型id
*/
#define REGISTE_CLASS_ID_RESOURCE(class_type)\
const long class_type::class_id = ++object::class_id;

/*----------------------------------------------------------------------------
* >>>>>>>>>>>>>>>>>>>>>>>>>>>>bean类注册宏(END)<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
* ---------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------
* >>>>>>>>>>>>>>>>>>>>>>>>>>反射方法注册宏(START)<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
* ---------------------------------------------------------------------------*/
//注意，以下几个宏中几个固定字符:class_name,obj,method_name,param1
#define FACTORY_INVOKE_START \
if(class_name==""){\
	return nullptr;\
}\

#define FACTORY_INVOKE(class_type) \
else if(#class_type==class_name){\
	return class_type::new_instance();\
}\

#define FACTORY_INVOKE_END \
else {\
	return nullptr;\
}\


/*
* 类型判断开始
*/
#define METHOD_INVOKE_CLASS_START(class_type)\
if(class_id==class_type::class_id){\
	class_type * __##class_type = (class_type*)obj;\
	if (method_name == "") {\
		std::cout<<"反射方法名不能为空"<<std::endl;\
		system("pause");\
		exit(0);\
	}

/*
* 类型判断结束
*/
#define METHOD_INVOKE_CLASS_END(class_type)\
	else {\
		std::cout<<"方法<"<<#class_type<<"."<<method_name<<">尚未注册"<<std::endl;\
        system("pause");\
		exit(0);\
	}\
}

/*
* 零元函数判断及调用
*/
#define METHOD_INVOKE_ZERO_PARAM(class_type, method)\
	else if(method_name == #method){\
		__##class_type -> method();\
	}

/*
* 一元函数判断及调用
*/
#define METHOD_INVOKE_ONE_PARAM(class_type, method)\
	else if(method_name == #method){\
		__##class_type -> method(param1);\
	}


/*----------------------------------------------------------------------------
* >>>>>>>>>>>>>>>>>>>>>>>>>>>反射方法注册宏(END)<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
* ---------------------------------------------------------------------------*/


