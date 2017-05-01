#pragma once
#include<vector>
#include<string>
#include<stdexcept>
#include<iostream>
#include<fstream>
#include"reflect/object.h"
#include"enumeration.h"

#define INVALID -1

class global_control_config :public object {
	REGISTE_MEMBER_HEAD(global_control_config)

	/*
	* 仿真时长
	*/
private:
	int m_ntti;
	void set_ntti(std::string t_ntti) {
		m_ntti = stoi(t_ntti);
	}
public:
	int get_ntti() {
		return m_ntti;
	}
};


class gtt_highspeed_config :public object {
	REGISTE_MEMBER_HEAD(gtt_highspeed_config)
	/*
	* 道路数量
	*/
private:
	const int m_road_num = 6;
public:
	int get_road_num() {
		return m_road_num;
	}

	/*
	* 路长,单位m
	*/
private:
	double m_road_length = INVALID;
	void set_road_length(std::string t_road_length) {
		m_road_length = stod(t_road_length);
	}
public:
	double get_road_length() {
		return m_road_length;
	}

	/*
	* 路宽，单位m
	*/
private:
	double m_road_width = INVALID;
	void set_road_width(std::string t_road_width) {
		m_road_width = stod(t_road_width);
	}
public:
	double get_road_width() {
		return m_road_width;
	}

	/*
	* 车速,km/h
	*/
private:
	double m_speed = INVALID;
	void set_speed(std::string t_speed) {
		m_speed = stod(t_speed);
	}
public:
	double get_speed() {
		return m_speed;
	}

	/*
	* 道路拓扑位置
	*/
private:
	const double m_road_topo_ratio[6 * 2]{
		0.0f, -2.5f,
		0.0f, -1.5f,
		0.0f, -0.5f,
		0.0f, 0.5f,
		0.0f, 1.5f,
		0.0f, 2.5f,
	};
public:
	const double* get_road_topo_ratio() {
		return m_road_topo_ratio;
	}

	/*
	* 车辆位置刷新时间,单位s
	*/
private:
	int m_freshtime = INVALID;
	void set_freshtime(std::string t_freshtime) {
		m_freshtime = stoi(t_freshtime);
	}
public:
	int get_freshtime() {
		return m_freshtime;
	}
};


class gtt_urban_config :public object {
	REGISTE_MEMBER_HEAD(gtt_urban_config)

	/*
	* 街区数量
	*/
private:
	const int m_road_num = 14;
public:
	int get_road_num() {
		return m_road_num;
	}

	/*
	* 路长，分为东西向和南北向,单位m
	*/
private:
	double m_road_length_ew = INVALID;
	double m_road_length_sn = INVALID;
	void set_road_length_ew(std::string t_road_length_ew) {
		m_road_length_ew = stod(t_road_length_ew);
	}
	void set_road_length_sn(std::string t_road_length_sn) {
		m_road_length_sn = stod(t_road_length_sn);
	}
public:
	double get_road_length_ew() {
		return m_road_length_ew;
	}
	double get_road_length_sn() {
		return m_road_length_sn;
	}

	/*
	* 路宽，单位m
	*/
private:
	double m_road_width = INVALID;
	void set_road_width(std::string t_road_width) {
		m_road_width = stod(t_road_width);
	}
public:
	double get_road_width() {
		return m_road_width;
	}

	/*
	* 车速,km/h
	*/
private:
	double m_speed = INVALID;
	void set_speed(std::string t_speed) {
		m_speed = stod(t_speed);
	}
public:
	double get_speed() {
		return m_speed;
	}

	/*
	* 道路拓扑位置
	*/
private:
	const double m_road_topo_ratio[14 * 2] = {
		-1.5f, 1.0f,
		-0.5f, 1.0f,
		0.5f, 1.0f,
		1.5f, 1.0f,
		-2.5f, 0.0f,
		-1.5f, 0.0f,
		-0.5f, 0.0f,
		0.5f, 0.0f,
		1.5f, 0.0f,
		2.5f, 0.0f,
		-1.5f,-1.0f,
		-0.5f,-1.0f,
		0.5f,-1.0f,
		1.5f,-1.0f
	};

private:
	const int m_wrap_around_road[14][9] = {
		{ 0,1,6,5,4,13,8,9,10 },
		{ 1,2,7,6,5,0,9,10,11 },
		{ 2,3,8,7,6,1,10,11,12 },
		{ 3,4,9,8,7,2,11,12,13 },
		{ 4,5,10,9,8,3,12,13,0 },
		{ 5,6,11,10,9,4,13,0,1 },
		{ 6,7,12,11,10,5,0,1,2 },
		{ 7,8,13,12,11,6,1,2,3 },
		{ 8,9,0,13,12,7,2,3,4 },
		{ 9,10,1,0,13,8,3,4,5 },
		{ 10,11,2,1,0,9,4,5,6 },
		{ 11,12,3,2,1,10,5,6,7 },
		{ 12,13,4,3,2,11,6,7,8 },
		{ 13,0,5,4,3,12,7,8,9 }
	};

public:
	const double* get_road_topo_ratio() {
		return m_road_topo_ratio;
	}
	const int(*get_wrap_around_road())[9]{
		return m_wrap_around_road;
	}
	/*
	* 车辆位置刷新时间,单位s
	*/
private:
	int m_freshtime = INVALID;
	void set_freshtime(std::string t_freshtime) {
		m_freshtime = stoi(t_freshtime);
	}
public:
	int get_freshtime() {
		return m_freshtime;
	}

};


class rrm_config :public object {
	REGISTE_MEMBER_HEAD(rrm_config)

	/*------------------静态成员------------------*/
public:
	/*
	* 每个RB的带宽(Hz)
	*/
	static const int s_BANDWIDTH_OF_RB = 12 * 1000 * 15;

	/*
	* 单位(个),由于RB带宽为180kHz，TTI为1ms，因此单位TTI单位RB传输的比特数为180k*1ms=180
	*/
	static const int s_BIT_NUM_PER_RB = 180;

	/*--------------------字段--------------------*/

	/*
	* 总带宽
	*/
private:
	int m_total_bandwidth;
	void set_total_bandwidth(std::string t_total_bandwidth) {
		m_total_bandwidth = stoi(t_total_bandwidth) * 1000 * 1000;
	}
public:
	int get_total_bandwidth() {
		return m_total_bandwidth;
	}

	/*
	* 一个可用资源块的rb数量
	*/
private:
	int m_rb_num_per_pattern;
	void set_rb_num_per_pattern(std::string t_rb_num_per_pattern) {
		m_rb_num_per_pattern = stoi(t_rb_num_per_pattern);
	}
public:
	int get_rb_num_per_pattern() {
		return m_rb_num_per_pattern;
	}

	/*
	* pattern数量
	*/
private:
	int m_pattern_num;
	void set_pattern_num() {
		m_pattern_num = get_total_bandwidth() / s_BANDWIDTH_OF_RB / get_rb_num_per_pattern();
	}
public:
	int get_pattern_num() {
		return m_pattern_num;
	}

	/*
	* 丢包临界sinr
	*/
private:
	double m_drop_sinr_boundary;
	void set_drop_sinr_boundary(std::string t_drop_sinr_boundary) {
		m_drop_sinr_boundary = stod(t_drop_sinr_boundary);
	}
public:
	double get_drop_sinr_boundary() {
		return m_drop_sinr_boundary;
	}

	/*
	* 选择算法
	*/
private:
	int m_select_altorithm;
	void set_select_altorithm(std::string t_select_altorithm) {
		m_select_altorithm = stoi(t_select_altorithm);
	}
public:
	int get_select_altorithm() {
		return m_select_altorithm;
	}

	/*
	* 时分粒度大小
	*/
private:
	int m_time_division_granularity;
	void set_time_division_granularity(std::string t_time_division_granularity) {
		m_time_division_granularity = stoi(t_time_division_granularity);
	}
public:
	int get_time_division_granularity() {
		return m_time_division_granularity;
	}
};


class tmc_config :public object {
	REGISTE_MEMBER_HEAD(tmc_config)

	/*
	* 事件包数
	*/
private:
	int m_package_num;
	void set_package_num(std::string t_package_num) {
		m_package_num = stoi(t_package_num);
	}
public:
	int get_package_num() {
		return m_package_num;
	}

	/*
	* Hello包数
	*/
private:
	int m_hello_tti;
	void set_hello_tti(std::string t_m_hello_tti) {
		m_hello_tti = stoi(t_m_hello_tti);
	}
public:
	int get_hello_tti() {
		return m_hello_tti;
	}

	/*
	*  事件触发概率
	*/
private:
	double m_trigger_rate;
	void set_trigger_rate(std::string t_trigger_rate) {
		m_trigger_rate = stod(t_trigger_rate);
	}
public:
	double get_trigger_rate() {
		return m_trigger_rate;
	}
};


class route_config :public object {
	REGISTE_MEMBER_HEAD(route_config)

	/*
	* Hello包发送平均周期
	*/
private:
	int m_interval;
	void set_interval(std::string t_interval) {
		m_interval = stoi(t_interval);
	}
public:
	int get_t_interval() {
		return m_interval;
	}
};