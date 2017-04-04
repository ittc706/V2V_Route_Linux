#pragma once

class context;

class system_control {
	/*------------------友元声明------------------*/
	/*
	* 将容器类作为友元，以提供依赖注入的权限
	*/
	friend class context;

	/*------------------私有字段------------------*/
private:
	/*
	* 持有容器的指针
	*/
	context* m_context = nullptr;
	void set_context(context* t_context);
public:
	context* get_context();


	/*----------------拷贝控制成员----------------*/
public:
	/*
	* 默认构造函数
	*/
	system_control();

	/*
	* 析构函数，负责清理资源
	*/
	~system_control();

	/*
	* 将拷贝构造函数定义为删除
	*/
	system_control(const system_control& t_system_control) = delete;

	/*
	* 将移动构造函数定义为删除
	*/
	system_control(system_control&& t_system_control) = delete;

	/*
	* 将拷贝赋值运算符定义为删除
	*/
	system_control& operator=(const system_control& t_system_control) = delete;

	/*
	* 将移动赋值运算符定义为删除
	*/
	system_control& operator=(system_control&& t_system_control) = delete;


	/*--------------------接口--------------------*/
public:
	/*
	* 系统运行唯一接口
	*/
	void process();

};