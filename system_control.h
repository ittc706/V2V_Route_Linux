#pragma once
#include"reflect/object.h"

class v2x_time;
class gtt;
class route;
class wt;
class tmc;
class global_control_config;

class system_control:public object {
	REGISTE_MEMBER_HEAD(system_control)

	/*------------------私有字段------------------*/
	/*
	* 仿真时刻对象
	*/
private:
	v2x_time* m_time;
	void set_time(object* t_time) {
		m_time = (v2x_time*)t_time;
	}
public:
	v2x_time* get_time() {
		return m_time;
	}

private:
	/*
	* gtt模块控制器
	*/
	gtt* m_gtt;
	void set_gtt(object* t_gtt) {
		m_gtt = (gtt*)t_gtt;
	}
public:
	gtt* get_gtt() {
		return m_gtt;
	}

	/*
	* route模块控制器
	*/
private:
	route* m_route;
	void set_route(object* t_route) {
		m_route = (route*)t_route;
	}
public:
	route* get_route() {
		return m_route;
	}

	/*
	* tmc模块控制器
	*/
private:
	tmc* m_tmc;
	void set_tmc(object* t_tmc) {
		m_tmc = (tmc*)t_tmc;
	}
public:
	tmc* get_tmc() {
		return m_tmc;
	}

	/*
	* wt模块控制器
	*/
private:
	wt* m_wt;
	void set_wt(object* t_wt) {
		m_wt = (wt*)t_wt;
	}
public:
	wt* get_wt() {
		return m_wt;
	}

	/*
	* 全局配置对象
	*/
private:
	global_control_config* m_global_control_config;
	void set_global_control_config(object* t_global_control_config) {
		m_global_control_config = (global_control_config*)t_global_control_config;
	}
public:
	global_control_config* get_global_control_config() {
		return m_global_control_config;
	}

	/*--------------------接口--------------------*/
public:
	/*
	* 系统运行唯一接口
	*/
	void process();

};