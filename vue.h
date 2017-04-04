#pragma once

/*
* 前置声明
*/
class vue_physics;

class vue {
	/*------------------私有字段------------------*/
private:
	/*
	* 指向车辆类物理层的实体
	*/
	vue_physics* m_physics_level = nullptr;


	/*----------------拷贝控制成员----------------*/
public: 
	/*
	* 默认构造函数，控制三层实体指针的初始化
	*/
	vue();

	/*
	* 析构函数，负责清理资源
	*/
	~vue();

	/*
	* 将拷贝构造函数定义为删除
	*/
	vue(const vue& t_vue) = delete;

	/*
	* 将移动构造函数定义为删除
	*/
	vue(vue&& t_vue) = delete;

	/*
	* 将拷贝赋值运算符定义为删除
	*/
	vue& operator=(const vue& t_vue) = delete;

	/*
	* 将移动赋值运算符定义为删除
	*/
	vue& operator=(vue&& t_vue) = delete;

	/*----------------访问与编辑器----------------*/
public:
	/*
	* 车辆物理层部分访问器
	*/
	vue_physics* get_physics_level();
};