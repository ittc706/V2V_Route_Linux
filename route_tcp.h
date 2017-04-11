#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<set>
#include<utility>
#include<random>
#include<string>
#include"route.h"
#include"context.h"
#include"config.h"

enum route_response_state{
	ACCEPT,
	REJECT
};

enum route_transimit_state {
	SUCCESS,
	FAILURE
};

enum route_tcp_pattern_state {
	TO_BE_SEND,//将要发送
	SENDING,//发送状态
	TO_BE_RECEIVE,//将要接收
	RECEIVING,//接受状态
	IDLE//空闲状态
};


/*
* 对于路由层，不涉及车辆，将车辆抽象为node
* 对于一个节点，收发矛盾。即同一时刻只能收，或者只能发
* 当一个节点处于收发状态时，回绝一切请求
* 当节点处于收状态时，该节点的作为源节点的信息也将在收完毕后再进行发送，即排在转发消息之后
* 若节点处于空闲状态，且同时收到两个或多个转发请求，随机应答一个，拒绝其他(可以扩展为优先级应答)
*/

class route_tcp_node;

class route_tcp_route_event {
private:
	static int s_event_count;

	/*
	* 源节点
	*/
private:
	const int m_origin_source_node_id;
public:
	int get_origin_source_node_id() { 
		return m_origin_source_node_id;
	}

	/*
	* 目的节点
	*/
private:
	const int m_final_destination_node_id;
public:
	int get_final_destination_node_id() { 
		return m_final_destination_node_id;
	}

	/*
	* 当前节点(已正确传输到当前节点，即当前节点也在m_through_node_vec之中)
	*/
private:
	int m_current_node_id = -1;
public:
	void set_current_node_id(int t_current_node_id) { 
		m_current_node_id = t_current_node_id;
		m_through_node_id_vec.push_back(m_current_node_id);
	}
	int get_current_node_id() { 
		return m_current_node_id;
	}

	/*
	* 经历的节点列表(只包含成功传输的)
	*/
private:
	std::vector<int> m_through_node_id_vec;
public:
	const std::vector<int>& get_through_node_vec() {
		return m_through_node_id_vec;
	}


	/*
	* 整个从源节点到目的节点是否传递成功
	*/
	bool is_finished() { 
		return m_current_node_id == m_final_destination_node_id; 
	}

	/*
	* 事件id
	*/
private:
	const int m_event_id;
public:
	int get_event_id() { return m_event_id; }

	/*
	* 数据包总数
	* 即要发送的数据封装成IP数据包的数量，这些IP数据包丢失任意一个，那么整个数据包就算丢失
	*/
private:
	const int m_package_num;
public:
	int get_package_num() {
		return m_package_num;
	}

public:
	/*
	* 构造函数，提供给事件触发模块调用
	*/
	route_tcp_route_event(int t_source_node, int t_destination_node) :
		m_event_id(s_event_count++), 
		m_origin_source_node_id(t_source_node),
		m_final_destination_node_id(t_destination_node),
		m_package_num(context::get_context()->get_tmc_config()->get_package_num()){
		set_current_node_id(t_source_node);
	}

	/*
	* 拷贝当前事件，拷贝前后event_id不变
	* 当某一跳成功传输，但是标记成功与否的信令传输失败，则会造成多条链路，此时才会调用clone
	*/
	void transfer_to(int t_node_id) {
		set_current_node_id(t_node_id);
	}

	/*
	* 转为字符串
	*/
	std::string to_string();
};

class route_tcp_link_event {
private:
	/*
	* 当前链路源节点id
	*/
	const int m_source_node_id;
public:
	int get_source_node_id() {
		return m_source_node_id;
	}

	/*
	* 当前链路目的节点id
	*/
private:
	const int m_destination_node_id;
public:
	int get_destination_node_id() {
		return m_destination_node_id;
	}

	/*
	* 数据包总数
	* 即要发送的数据封装成IP数据包的数量，这些IP数据包丢失任意一个，那么整个数据包就算丢失
	*/
private:
	const int m_package_num;

	/*
	* 占用的pattern编号
	*/
private:
	int m_pattern_idx;
	void set_pattern_idx(int t_pattern_idx) {
		m_pattern_idx = t_pattern_idx;
	}
public:
	int get_pattern_idx() {
		return m_pattern_idx;
	}

	/*
	* 标记本跳当前时刻传输的包编号
	*/
private:
	int m_package_idx = 0;
public:
	int get_package_idx() { return m_package_idx; }

	/*
	* 标记本跳是否传输完毕(无论是否发生丢包)
	*/
private:
	bool m_is_finished = false;
public:
	bool is_finished() { return m_is_finished; }

	/*
	* 标记本跳是否发生丢包
	*/
private:
	bool m_is_loss = false;
public:
	bool get_is_loss() { return m_is_loss; }

public:
	route_tcp_link_event(int t_source_node_id, int t_destination_node_id, int t_pattern_idx, int t_package_num) :
		m_source_node_id(t_source_node_id),
		m_destination_node_id(t_destination_node_id),
		m_pattern_idx(t_pattern_idx),
		m_package_num(t_package_num) {}

	/*
	* 进行数据包的发送
	*/
	void transimit();
};

class route_tcp_node {
	friend class route_tcp;

private:
	/*
	* 节点编号
	*/
	static int s_node_count;

	/*
	* 随机数引擎
	*/
	static std::default_random_engine s_engine;

	/*
	* 日志输出流
	*/
	static std::ofstream s_logger;

	/*
	* 正在发送(强调一下:发状态的节点)的node节点
	* 外层下标为pattern编号
	*/
	static std::vector<std::set<route_tcp_node*>> s_node_per_pattern;

	/*
	* 当前节点待发送车辆队列
	*/
private:
	std::queue<route_tcp_route_event*> m_send_event_queue;
public:
	void offer_send_event_queue(route_tcp_route_event* t_event) {
		m_send_event_queue.push(t_event);
	}
	route_tcp_route_event* poll_send_event_queue() {
		route_tcp_route_event* temp = m_send_event_queue.front();
		m_send_event_queue.pop();
		return temp;
	}
	route_tcp_route_event* peek_send_event_queue() {
		return m_send_event_queue.front();
	}
	bool is_send_event_queue_empty() {
		return m_send_event_queue.empty();
	}

private:
	/*
	* 发送ack请求时，会创建link_event，并将其添加到该结构中
	* 并在下一tti进行传输
	* 外层下标为pattern
	* 仅用于该节点作为relay时(同时维护该链路source节点的m_pattern_state)
	*/
	std::vector<route_tcp_link_event*> m_tobe_link_transimit;

	/*
	* 当前节点，当前时刻，每个pattern的使用情况
	* 若pair的first字段，即pattern状态为IDLE，那么内层pair的second字段无效
	* pair的second字段为链路事件指针
	*/
private:
	std::vector<std::pair<route_tcp_pattern_state, route_tcp_link_event*>> m_pattern_state;

private:
	/*
	* 发送response时，如果传输成功，将route_event添加到该结构
	* 在下一刻刷新到m_send_event_queue中
	* 外层下标为pattern
	*/
	std::vector<route_tcp_route_event*> m_tobe_relay;

	/*
	* 当前节点，每个频段上收到来自其他车辆的syn请求
	*/
private:
	std::vector<std::vector<int>> m_syn_request_per_pattern;

private:
	/*
	* 本次tti收到的syn请求，将于下个tti进行ack处理
	*/
	std::vector<std::vector<int>> m_tobe_syn_request_per_pattern;
public:
	//添加syn请求
	void add_syn_request(int t_pattern_idx, int t_source_node_id) {
		m_tobe_syn_request_per_pattern[t_pattern_idx].push_back(t_source_node_id);
	}

	/*
	* 保存上一次请求的relay_node_id以及pattern_idx
	* (-1,-1)则是无效状态
	* 如果缓存不为空，说明该节点正在等待ack！！！
	*/
private:
	std::pair<int, int> m_select_cache;
public:
	void clear_select_cache() {
		m_select_cache.first = -1;
		m_select_cache.second = -1;
	}

private:
	/*
	* 节点id
	*/
	const int m_id = s_node_count++;
public:
	int get_id() {
		return m_id;
	}

	/*
	* 邻接列表
	*/
private:
	std::vector<int> m_adjacent_list;
public:
	void add_to_adjacent_list(int t_node_id) {
		m_adjacent_list.push_back(t_node_id);
	}
	const std::vector<int>& get_adjacent_list() {
		return m_adjacent_list;
	}


public:
	/*
	* 构造函数
	*/
	route_tcp_node();

public:
	/*
	* 选择请求转发的车辆以及相应的频段
	* first字段为车辆id
	* second字段为频段编号
	* 任意一个字段为-1，就代表选择失败
	*/
	std::pair<int,int> select_relay_information();
};

class route_tcp :public route {
	/*
	* 让context容器提供依赖注入
	*/
	friend class context;

private:
	/*
	* 随机数引擎
	*/
	static std::default_random_engine s_engine;

	/*
	* 日志输出流
	*/
	static std::ofstream s_logger;

private:
	static void log_event_trigger(int t_origin_node_id, int t_fianl_destination_node_id);

	static void log_link_event_state(int t_source_node_id, int t_relay_node_id, std::string t_description);
private:
	/*
	* 节点数组
	*/
	route_tcp_node* m_node_array;
public:
	route_tcp_node* get_node_array() { 
		return m_node_array; 
	}

private:
	/*
	* 成功/失败传输的事件
	*/
	std::vector<route_tcp_route_event*> m_successful_event_vec;
	std::vector<route_tcp_link_event*> m_failed_event_vec;
	void add_successful_event(route_tcp_route_event* t_successful_event_vec) {
		m_successful_event_vec.push_back(t_successful_event_vec);
	}
	void add_failed_event(route_tcp_link_event* t_failed_event_vec) {
		m_failed_event_vec.push_back(t_failed_event_vec);
	}
public:
	const std::vector<route_tcp_route_event*>& get_successful_event_vec() {
		return m_successful_event_vec;
	}
	const std::vector<route_tcp_link_event*>& get_failed_event_vec(){
		return m_failed_event_vec;
	}

public:
	/*
	* 构造函数
	*/
	route_tcp();

	/*
	* 初始化
	*/
	void initialize()override;

	/*
	* 对整个网络层进行状态更新，对外暴露的接口，每个TTI调用一次即可
	*/
	void process_per_tti()override;

	/*
	* 随车辆运动而更新邻接列表，车辆刷新时调用即可
	*/
	void update_route_table_from_physics_level()override;

private:
	/*
	* 随机触发事件
	*/
	void event_trigger();

	/*
	* 发送syn
	*/
	void send_syn();

	/*
	* 发送ack
	*/
	void send_ack();

	/*
	* 进行接收(收发两端，仅以收端作为处理点，发端在收端处理的同时一并处理)
	*/
	void receive_data();

	/*
	* 在tti开始时，刷新tobe数据结构
	*/
	void update_tobe();
};



