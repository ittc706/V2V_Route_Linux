#pragma once

#include<set>
#include<random>
#include<utility>
#include<memory>
#include"reflect/object.h"

class sender_event;

class wt :public object{
	REGISTE_MEMBER_HEAD(wt)

	/*---------------静态成员---------------*/
private:
	/*
	* 随机数引擎，该类共享
	*/
	static std::default_random_engine s_engine;

	/*
	* 共享资源
	*/
	static std::vector<double>* m_qpsk_mi;

	/*
	* 公共资源资源初始化，交由容器调用
	*/
	static void set_resource();
	/*----------------字段------------------*/
private:
	/*
	* 发送天线数
	*/
	int m_nt = 1;

	/*
	* 接收天线数
	*/
	int m_nr = 2;

	/*
	* 发射功率
	*/
	double m_pt;

	/*
	* 距离路径损耗
	*/
	double m_ploss;

	/*
	* 干扰距离路径损耗
	*/
	std::vector<double> m_inter_ploss;

	/*
	* 高斯噪声的功率，单位是mw
	*/
	double m_sigma;

	/*------------------接口------------------*/
public:
	/*
	* 计算载干比
	* t_send_vue_id:当前链路的发送端车辆id
	* t_receive_vue_id:当前链路的接收端车辆id
	* t_pattern_idx:当前占用的pattern_idx
	* t_sending_vue_id_set:在该子载波区间进行发送的车辆id列表，包括t_send_vue_id
	*/
	double calculate_sinr(int t_send_vue_id, int t_receive_vue_id, int t_pattern_idx, const std::set<int>& t_sending_vue_id_set);

private:
	/*
	* 二分法查找算法
	*/
	int closest(std::vector<double> t_vec, double t_target);

	/*
	* 查表
	*/
	double get_mutual_information(std::vector<double> t_vec, int t_index);
};