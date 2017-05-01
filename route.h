#pragma once
#include"enumeration.h"
#include"reflect/object.h"

enum route_transimit_state {
	SUCCESS,
	FAILURE
};

struct event_trigger_dto {
	int source_vue_id;
	int destination_vue_id;
};

class v2x_time;
class gtt;
class wt;
class rrm_config;
class tmc_config;
class route_config;

class route:public object {
public:
	/*
	* 获取时间对象
	*/
	virtual v2x_time* get_time() = 0;

	/*
	* 获取gtt控制器
	*/
	virtual gtt* get_gtt() = 0;

	/*
	* 获取wt控制器
	*/
	virtual wt* get_wt() = 0;

	/*
	* 获取无线资源管理配置对象
	*/
	virtual rrm_config* get_rrm_config() = 0;

	/*
	* 获取事件配置对象
	*/
	virtual tmc_config* get_tmc_config() = 0;

	/*
	* 获取路由配置对象
	*/
	virtual route_config* get_route_config() = 0;

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