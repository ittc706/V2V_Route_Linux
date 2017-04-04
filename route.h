#pragma once
#include"enumeration.h"

struct event_trigger_dto {
	int source_vue_id;
	int destination_vue_id;
};

class route {
	/*------------------友元声明------------------*/
	/*
	* 将context设为友元，容器要为其注入依赖项
	*/
	friend class context;

	/*--------------------静态--------------------*/
	/*
	* 根据gtt模式来生成gtt组件对象
	*/
	static route* route_bind_by_mode(route_mode t_mode);
public:
	/*
	* 初始化
	*/
	virtual void initialize() = 0;

	/*
	* 对整个网络层进行状态更新，对外暴露的接口，每个TTI调用一次即可
	*/
	virtual void process_per_tti() = 0;

	/*
	* 随车辆运动而更新邻接列表，车辆刷新时调用即可
	*/
	virtual void update_route_table_from_physics_level() = 0;
};