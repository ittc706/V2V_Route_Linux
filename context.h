#pragma once
#include<vector>
#include<list>

class system_control;
class config_loader;
class global_control_config;
class gtt_config;
class rrm_config;
class tmc_config;
class route_config;
class gtt;
class tmc;
class wt;
class vue;
class sender_event;
class route;

class context {
	/*------------------友元声明------------------*/
	/*
	* 将system_control设为context的友元，提供其构造容器类唯一实例的权限
	*/
	friend class system_control;
	friend class gtt_highspeed;
	friend class gtt_urban;
	/*------------------静态成员字段------------------*/
private:
	/*
	* 单例模式下，唯一实体的指针
	*/
	static context* s_singleton_context;

public:
	/*
	* 单例模式下，获取唯一实体的指针
	*/
	static context* get_context();

	/*----------------拷贝控制成员----------------*/
private:
	/*
	* 默认构造函数
	*/
	context() {}

	/*
	* 读取配置文件
	*/
	void read_configuration();

	/*
	* 单例对象生成
	*/
	void singleton_initialize();

	/*
	* 依赖注入
	*/
	void dependcy_inject();

	/*
	* 对象生成，以及依赖注入以后，执行的对象的初始化方法
	*/
	void post_processor();

public:
	/*
	* 析构函数，负责清理资源
	*/
	~context();

	/*
	* 将拷贝构造函数定义为删除
	*/
	context(const context& t_context) = delete;

	/*
	* 将移动构造函数定义为删除
	*/
	context(context&& t_context) = delete;

	/*
	* 将拷贝赋值运算符定义为删除
	*/
	context& operator=(const context& t_context) = delete;

	/*
	* 将移动赋值运算符定义为删除
	*/
	context& operator=(context&& t_context) = delete;

	/*------------------容器成员------------------*/
	/*
	* 系统控制器对象、编辑器、访问器
	*/
private:
	system_control* m_system_control = nullptr;
	void set_system_control(system_control* t_system_control) {
		m_system_control = t_system_control;
	}
public:
	system_control* get_system_control() {
		return m_system_control;
	}

	/*
	* 配置文件加载对象、编辑器、访问器
	*/
private:
	config_loader* m_config_loader = nullptr;
	void set_config_loader(config_loader* t_config_loader) {
		m_config_loader = t_config_loader;
	}
public:
	config_loader* get_config_loader() {
		return m_config_loader;
	}

	/*
	* global_control配置参数对象
	*/
private:
	global_control_config* m_global_control_config = nullptr;
	void set_global_control_config(global_control_config* t_global_control_config) {
		m_global_control_config = t_global_control_config;
	}
public:
	global_control_config* get_global_control_config() {
		return m_global_control_config;
	}

	/*
	* gtt配置参数对象
	*/
private:
	gtt_config* m_gtt_config = nullptr;
	void set_gtt_config(gtt_config* t_gtt_config) {
		m_gtt_config = t_gtt_config;
	}
public:
	gtt_config* get_gtt_config() {
		return m_gtt_config;
	}

	/*
	* rrm配置参数对象
	*/
private:
	rrm_config* m_rrm_config = nullptr;
	void set_rrm_config(rrm_config* t_rrm_config) {
		m_rrm_config = t_rrm_config;
	}
public:
	rrm_config* get_rrm_config() {
		return m_rrm_config;
	}

	/*
	* tmc配置参数对象
	*/
private:
	tmc_config* m_tmc_config = nullptr;
	void set_tmc_config(tmc_config* t_tmc_config) {
		m_tmc_config = t_tmc_config;
	}
public:
	tmc_config* get_tmc_config() {
		return m_tmc_config;
	}

	/*
	* route配置参数对象
	*/
private:
	route_config* m_route_config;
	void set_route_config(route_config* t_route_config) {
		m_route_config = t_route_config;
	}
public:
	route_config* get_route_config() {
		return m_route_config;
	}

	/*
	* tti,仿真时刻
	*/
private:
	int m_tti = 0;
public:
	void increase_tti() {
		++m_tti;
	}
	int get_tti() {
		return m_tti;
	}

	/*
	* route
	*/
private:
	route* m_route;
	void set_route(route* t_route) {
		m_route = t_route;
	}
public:
	route* get_route() {
		return m_route;
	}

	/*
	* gtt实体指针
	*/
private:
	gtt* m_gtt = nullptr;
	void set_gtt(gtt* t_gtt) {
		m_gtt = t_gtt;
	}
public:
	gtt* get_gtt() {
		return m_gtt;
	}

	/*
	* tmc实体指针
	*/
private:
	tmc* m_tmc = nullptr;
	void set_tmc(tmc* t_tmc) {
		m_tmc = t_tmc;
	}
public:
	tmc* get_tmc() {
		return m_tmc;
	}

	/*
	* wt对象，为非单例模式，可以请求数个wt类型的对象
	*/
private:
	wt* m_wt = nullptr;
	void set_wt(wt* t_wt) {
		m_wt = t_wt;
	}
public:
	wt* get_wt() {
		return m_wt;
	}

	/*
	* 车辆类数组指针
	*/
private:
	vue* m_vue_array = nullptr;
	void set_vue_array(vue* t_vue_array) {
		m_vue_array = t_vue_array;
	}
public:
	vue* get_vue_array() {
		return m_vue_array;
	}
};

