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

enum route_tcp_node_state {
	/*
	* 以下状态的值都是设计过的，请不要随意调整
	*/

	IDLE = 0,//空闲状态

	SOURCE_SEND_SYN = 3,//source节点发送转发请求给relay节点
	SOURCE_RECEIVE_ACK = 2,//source节点接收relay节点的应答信号
	SOURCE_SENDING = 4,//source节点正在转发数据包
	SOURCE_LINK_RESPONSE = 5,//source节点接收relay节点的传输是否成功的响应

	RELAY_SEND_ACK = 1,//relay节点发送应答信号
	RELAY_RECEIVING = 6,//relay节点正在接收数据包
	RELAY_LINK_RESPONSE = 7//relay节点发送传输是否成功

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
	route_tcp_route_event* clone();

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
	route_tcp_link_event(int t_source_node_id, int t_destination_node_id, int t_package_num) :
		m_source_node_id(t_source_node_id),
		m_destination_node_id(t_destination_node_id),
		m_package_num(t_package_num) {}

	/*
	* 进行数据包的发送
	*/
	void transimit();
};

class route_tcp_source_node {
	friend class route_tcp_node;
private:
	route_tcp_source_node() {}
	/*
	* 待路由转发事件列表
	* 当前节点作为source节点的事件队列，当且仅当：当前节点处于转发状态，并且触发信的事件，此时队列长度为2
	* 其他任何时候队列中只有触发事件或者转发事件
	*/
private:
	std::queue<route_tcp_route_event*> m_event_queue;
public:
	void offer_event(route_tcp_route_event* t_event) {
		m_event_queue.push(t_event);
	}
	route_tcp_route_event* poll_event() {
		route_tcp_route_event* temp = m_event_queue.front();
		m_event_queue.pop();
		return temp;
	}
	route_tcp_route_event* peek_event() {
		return m_event_queue.front();
	}
};

class route_tcp_relay_node {
	friend class route_tcp_node;
private:
	route_tcp_relay_node() {}
	/*
	* 当前节点作为relay节点的事件队列
	*/
private:
	std::queue<route_tcp_link_event*> m_event_queue;
public:
	void offer_event(route_tcp_link_event* t_event) {
		m_event_queue.push(t_event);
	}
	route_tcp_link_event* poll_event() {
		route_tcp_link_event* temp = m_event_queue.front();
		m_event_queue.pop();
		return temp;
	}
	route_tcp_link_event* peek_event() {
		return m_event_queue.front();
	}

	/*
	* 当前节点接收来自其他车辆的syn请求的列表(上一个tti)
	*/
private:
	std::vector<int> m_pre_syn_node_vec;

	/*
	* 当前节点接收来自其他车辆的syn请求的列表(当前tti)
	*/
private:
	std::vector<int> m_syn_node_vec;
public:
	void add_relay_syn_node(int t_node_id) {
		m_syn_node_vec.push_back(t_node_id);
	}
};

class route_tcp_node {
	friend class route_tcp;

private:
	static int s_node_count;
	static std::default_random_engine s_engine;

	static std::ofstream s_logger;

	static void log_state_update(route_tcp_node* t_node);
	/*
	* 正在发送(强调一下:发状态的节点)的node节点
	* 外层下标为pattern编号
	*/
	static std::vector<std::set<route_tcp_node*>> s_node_per_pattern;

private:
	/*
	*  指向代表该节点source状态的对象
	*/
	route_tcp_source_node* m_source_node;
	route_tcp_source_node* get_source_node() {
		return m_source_node;
	}

	/*
	*  指向代表该节点relay状态的对象
	*/
	route_tcp_relay_node* m_relay_node;
	route_tcp_relay_node* get_relay_node(){
		return m_relay_node;
	}

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

	/*
	* 节点当前状态状态
	*/
private:
	route_tcp_node_state m_cur_state = IDLE;
	void set_cur_state(route_tcp_node_state t_cur_state) {
		m_cur_state = t_cur_state; 
	}
public:
	route_tcp_node();

	/*
	* 返回节点当前时刻的状态
	*/
	route_tcp_node_state get_cur_state() {
		return m_cur_state; 
	}

	/*
	* 节点下一刻可能的状态集合
	*/
private:
	std::set<route_tcp_node_state> m_next_possible_state_set;
	void add_next_possible_state(route_tcp_node_state t_next_state) { 
		m_next_possible_state_set.insert(t_next_state);
	}

public:
	/*
	* 更新节点状态
	*/
	void update_state();

	/*
	* 选择请求转发的车辆
	*/
	int source_select_relay_node();

	/*
	* 对所有syn请求，返回相应的ack
	* 返回结果若为res,则向res.first节点发送ack=接收，向res.second节点发送ack=不接受
	* 若无法响应任何节点，那么res.first返回-1
	*/
	std::pair<int,std::vector<int>> relay_response_ack();

private:
	void check_state();
};

class route_tcp :public route {
	/*
	* 让context容器提供依赖注入
	*/
	friend class context;

private:
	static std::default_random_engine s_engine;

	static std::ofstream s_logger;
public:
	static std::string state_to_string(route_tcp_node_state state);
private:
	static void log_possible_state(int source_node_id, int relay_node_id, 
		int cur_node_id, route_tcp_node_state cur_state, 
		route_tcp_node_state next_state, std::string description);

	static void log_event_trigger(int t_origin_node_id, int t_fianl_destination_node_id);

	static void log_link_event_state(int t_source_node_id, int t_relay_node_id, std::string t_description);
private:
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
	* 处理请求连接
	*/
	void process_syn_connection();

	/*
	* 处理求情响应
	*/
	void process_ack_connection();

	/*
	* 处理转发数据包
	*/
	void process_transimit_connection();

	/*
	* 返回成功传输信息
	*/
	void process_response_connection();

	/*
	* 更新节点状态
	*/
	void update_node_state();
};



