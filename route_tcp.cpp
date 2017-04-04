#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include"route_tcp.h"
#include"context.h"
#include"config.h"
#include"gtt.h"
#include"vue.h"
#include"vue_physics.h"
#include"function.h"
using namespace std;

int route_tcp_route_event::s_event_count = 0;

route_tcp_route_event* route_tcp_route_event::clone() {
	route_tcp_route_event* clone_event = new route_tcp_route_event(get_origin_source_node_id(),get_final_destination_node_id());
	clone_event->m_through_node_id_vec = this->m_through_node_id_vec;
	return clone_event;
}

std::string route_tcp_route_event::to_string() {
	stringstream ss;
	for (int i = 0; i < m_through_node_id_vec.size();i++) {
		ss << "node[" << left << setw(3) << m_through_node_id_vec[i] << "]";
		if (i < m_through_node_id_vec.size() - 1)ss << " -> ";
	}
	ss << endl;
	return ss.str();
}

void route_tcp_link_event::transimit() {
	//<Warn>:一跳就在指定频段传输，不再分成多个包分别选择频段传输了

	if (++m_package_idx == m_package_num) {
		m_is_finished = true;
	}
	double sinr = -1;
	//<Warn>:sinr计算待补充，以及是否丢包字段的维护
}

int route_tcp_node::s_node_count = 0;

default_random_engine route_tcp_node::s_engine;

ofstream route_tcp_node::s_logger;

void route_tcp_node::log_state_update(route_tcp_node* t_node) {
	s_logger << "TTI[" << left << setw(3) << context::get_context()->get_tti() << "] - ";
	s_logger << "node[" << left << setw(3) << t_node->get_id() << "] - ";
	s_logger << "curr_state[" << left << setw(20) << route_tcp::state_to_string(t_node->get_cur_state()) << "] - ";
	s_logger << "next_state[";
	for (route_tcp_node_state state : t_node->m_next_possible_state_set) {
		s_logger << left << setw(20) << route_tcp::state_to_string(state);
	}
	s_logger << "]" << endl;
}

std::vector<std::set<route_tcp_node*>> route_tcp_node::s_node_per_pattern;

route_tcp_node::route_tcp_node() {
	m_source_node = new route_tcp_source_node();
	m_relay_node = new route_tcp_relay_node();
}

void route_tcp_node::update_state() {
	route_tcp_node_state max_state = IDLE;

	log_state_update(this);

	check_state();

	for (route_tcp_node_state state : m_next_possible_state_set) {
		if (state > max_state) {
			max_state = state;
		}
	}

	if (max_state == IDLE) {
		if (get_source_node()->m_event_queue.size() != 0) {
			max_state = SOURCE_SEND_SYN;
		}
	}

	if (max_state == SOURCE_SEND_SYN) {
		if (get_source_node()->m_event_queue.size() == 0) {
			cout << "node[" << get_id() << "] possible list: ";
			for (int i : m_next_possible_state_set)
				cout << i << ",";
			cout << endl;
		}
	}

	set_cur_state(max_state);
	m_next_possible_state_set.clear();
}

int route_tcp_node::source_select_relay_node() {
	int res = -1;

	int final_destination_node_id = get_source_node()->peek_event()->get_final_destination_node_id();

	double min_distance = (numeric_limits<double>::max)();
	for (int near_node_id : m_adjacent_list) {
		int cur_distance = vue_physics::get_distance(near_node_id, final_destination_node_id);
		if (cur_distance< min_distance) {
			min_distance = cur_distance;
			res = near_node_id;
		}
	}

	return res;
}

std::pair<int, std::vector<int>> route_tcp_node::relay_response_ack() {
	/*
	* 无论节点处于何种状态，都必须处理这个syn请求列表，并且处理的是上一时刻的请求列表
	*/
	int select_node_id = -1;
	vector<int> reject_vec;

	if (get_cur_state() != RELAY_SEND_ACK) {
		reject_vec = get_relay_node()->m_pre_syn_node_vec;
	}
	else {
		uniform_int_distribution<int> u(0, get_relay_node()->m_pre_syn_node_vec.size() - 1);

		select_node_id = get_relay_node()->m_pre_syn_node_vec[u(s_engine)];

		for (int node_id : get_relay_node()->m_pre_syn_node_vec) {
			if (select_node_id != node_id) {
				reject_vec.push_back(node_id);
			}
		}
	}

	get_relay_node()->m_pre_syn_node_vec = get_relay_node()->m_syn_node_vec;//将当前tti收到的syn列表进行转移，留到下一个tti才会处理
	get_relay_node()->m_syn_node_vec.clear();//由于转移了，因此清空即可

	return std::pair<int, std::vector<int>>(select_node_id, reject_vec);
}

void route_tcp_node::check_state() {
	if (get_cur_state() == IDLE) {
		if (m_next_possible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_SENDING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_RECEIVING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_LINK_RESPONSE) != m_next_possible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == SOURCE_SEND_SYN) {
		if (m_next_possible_state_set.find(IDLE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_SENDING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_RECEIVING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_LINK_RESPONSE) != m_next_possible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == SOURCE_RECEIVE_ACK) {
		if (m_next_possible_state_set.find(IDLE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_RECEIVING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_LINK_RESPONSE) != m_next_possible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == SOURCE_SENDING) {
		if (m_next_possible_state_set.find(IDLE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_SEND_SYN) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_RECEIVING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_LINK_RESPONSE) != m_next_possible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == SOURCE_LINK_RESPONSE) {
		if (m_next_possible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_SENDING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_RECEIVING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_LINK_RESPONSE) != m_next_possible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == RELAY_SEND_ACK) {
		if (m_next_possible_state_set.find(IDLE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_SEND_SYN) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_SENDING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_LINK_RESPONSE) != m_next_possible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == RELAY_RECEIVING) {
		if (m_next_possible_state_set.find(IDLE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_SEND_SYN) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_SENDING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_possible_state_set.end())
			throw logic_error("state_error");
	}
	else if (get_cur_state() == RELAY_LINK_RESPONSE) {
		if (m_next_possible_state_set.find(SOURCE_RECEIVE_ACK) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_SENDING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(SOURCE_LINK_RESPONSE) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_RECEIVING) != m_next_possible_state_set.end() ||
			m_next_possible_state_set.find(RELAY_LINK_RESPONSE) != m_next_possible_state_set.end())
			throw logic_error("state_error");
	}
}

default_random_engine route_tcp::s_engine;

ofstream route_tcp::s_logger;

string route_tcp::state_to_string(route_tcp_node_state state) {
	switch (state) {
	case IDLE:
		return "IDLE";
	case SOURCE_SEND_SYN:
		return "SOURCE_SEND_SYN";
	case SOURCE_RECEIVE_ACK:
		return "SOURCE_RECEIVE_ACK";
	case SOURCE_SENDING:
		return "SOURCE_SENDING";
	case SOURCE_LINK_RESPONSE:
		return "SOURCE_LINK_RESPONSE";
	case RELAY_SEND_ACK:
		return "RELAY_SEND_ACK";
	case RELAY_RECEIVING:
		return "RELAY_RECEIVING";
	case RELAY_LINK_RESPONSE:
		return "RELAY_LINK_RESPONSE";
	}
}

void route_tcp::log_possible_state(int t_source_node_id, int t_relay_node_id, int t_cur_node_id, route_tcp_node_state t_cur_state, route_tcp_node_state t_next_state, string t_description) {
	s_logger << "TTI[" << left << setw(3) << context::get_context()->get_tti() << "] - ";
	s_logger << "link[" << left << setw(3) << t_source_node_id << ", ";
	s_logger << left << setw(3) << t_relay_node_id << "] - ";
	s_logger << "node[" << left << setw(3) << t_cur_node_id << "] - ";
	s_logger << "curr_state[" << left << setw(20) << state_to_string(t_cur_state) << "] - ";
	s_logger << "next_state[" << left << setw(20) << state_to_string(t_next_state) << "] - ";
	s_logger << t_description << endl;
}

void route_tcp::log_event_trigger(int t_origin_node_id, int t_fianl_destination_node_id) {
	s_logger << "TTI[" << left << setw(3) << context::get_context()->get_tti() << "] - ";
	s_logger << "trigger[" << left << setw(3) << t_origin_node_id << ", ";
	s_logger << left << setw(3) << t_fianl_destination_node_id << "]" << endl;

}

void route_tcp::log_link_event_state(int t_source_node_id, int t_relay_node_id, std::string t_description) {
	s_logger << "TTI[" << left << setw(3) << context::get_context()->get_tti() << "] - ";
	s_logger << "link[" << left << setw(3) << t_source_node_id << ", ";
	s_logger << left << setw(3) << t_relay_node_id << "] - ";
	s_logger << "{" << t_description << "}" << endl;
}


route_tcp::route_tcp() {

}

void route_tcp::initialize() {
	context* __context = context::get_context();
	int vue_num = __context->get_gtt()->get_vue_num();
	m_node_array = new route_tcp_node[vue_num];

	if (__context->get_global_control_config()->get_platform() == Windows) {
		s_logger.open("log\\route_tcp_log.txt");
		route_tcp_node::s_logger.open("log\\route_tcp_node_state_log.txt");
	}
	else {
		s_logger.open("log/route_tcp_log.txt");
		route_tcp_node::s_logger.open("log/route_tcp_node_state_log.txt");
	}
}

void route_tcp::process_per_tti() {
	//事件触发
	event_trigger();

	//更新节点状态
	update_node_state();

	//处理那些处于syn状态的节点
	process_syn_connection();

	//处理那些需要回复ack信号的节点
	process_ack_connection();

	//处理那些正在传输状态的节点
	process_transimit_connection();

	//处理那些正在回复传输标志的节点
	process_response_connection();
}

void route_tcp::update_route_table_from_physics_level() {
	//清除之前的邻接表
	for (int node_id = 0; node_id < route_tcp_node::s_node_count; node_id++) {
		get_node_array()[node_id].m_adjacent_list.clear();
	}

	//<Warn>:暂时改为根据距离确定邻接表
	context* __context = context::get_context();
	vue* vue_ary = __context->get_vue_array();
	int vue_num = __context->get_gtt()->get_vue_num();
	for (int vue_id_i = 0; vue_id_i < vue_num; vue_id_i++) {
		for (int vue_id_j = 0; vue_id_j < vue_num; vue_id_j++) {
			if (vue_id_i == vue_id_j)continue;
			if (vue_physics::get_distance(vue_id_i, vue_id_j) < 500) {
				get_node_array()[vue_id_i].add_to_adjacent_list(vue_id_j);
				get_node_array()[vue_id_j].add_to_adjacent_list(vue_id_i);
			}
		}
	}
}

void route_tcp::event_trigger() {
	context* __context = context::get_context();
	double trigger_rate = __context->get_tmc_config()->get_trigger_rate();

	uniform_real_distribution<double> u_rate(0, 1);
	uniform_int_distribution<int> u_node_id(0, route_tcp_node::s_node_count - 1);

	for (int origin_source_node_id = 0; origin_source_node_id < route_tcp_node::s_node_count; origin_source_node_id++) {
		if (u_rate(s_engine) < trigger_rate) {

			int final_destination_node_id = origin_source_node_id;
			while (final_destination_node_id == origin_source_node_id) {
				final_destination_node_id = u_node_id(s_engine);
			}
			get_node_array()[origin_source_node_id].get_source_node()->offer_event(
				new route_tcp_route_event(origin_source_node_id, final_destination_node_id)
				);
			log_event_trigger(origin_source_node_id, final_destination_node_id);
		}
	}
}

void route_tcp::process_syn_connection() {
	//处理SYN请求，在本次tti内，完成SYN的发送以及接收
	for (int source_node_id = 0; source_node_id < route_tcp_node::s_node_count; source_node_id++) {
		if (get_node_array()[source_node_id].get_cur_state() == SOURCE_SEND_SYN) {
			int relay_node_id = get_node_array()[source_node_id].source_select_relay_node();
			if (relay_node_id == -1) {
				//没有可用的中继节点
				get_node_array()[source_node_id].add_next_possible_state(SOURCE_SEND_SYN);
				continue;
			}
			get_node_array()[relay_node_id].get_relay_node()->add_relay_syn_node(source_node_id);

			//设置source-relay节点的下一刻可能状态
			get_node_array()[source_node_id].add_next_possible_state(SOURCE_RECEIVE_ACK);
			log_possible_state(source_node_id, relay_node_id, source_node_id,
				get_node_array()[source_node_id].get_cur_state(), SOURCE_RECEIVE_ACK,
				"process_syn_connection：发送syn请求，准等待ack");

			get_node_array()[relay_node_id].add_next_possible_state(RELAY_SEND_ACK);
			log_possible_state(source_node_id, relay_node_id, relay_node_id,
				get_node_array()[relay_node_id].get_cur_state(), RELAY_SEND_ACK,
				"process_syn_connection：未知状态，准备发送ack");
		}
	}
}

void route_tcp::process_ack_connection() {
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		/*没有进行判断(任意状态下都能进行ack应答)*/
		pair<int, vector<int>> ack_response = get_node_array()[relay_node_id].relay_response_ack();

		int accept_node_id = ack_response.first;
		if (accept_node_id == -1) {
			/*此时relay节点可能正处于发送状态，不为其添加下一刻任何可能的状态,下一刻状态由传输函数保证*/
			/*此时relay节点可能处于空闲状态*/
		}
		else {
			//建立传输的link_event
			int package_num = get_node_array()[accept_node_id].get_source_node()->peek_event()->get_package_num();
			route_tcp_link_event* link_event = new route_tcp_link_event(accept_node_id, relay_node_id, package_num);
			get_node_array()[relay_node_id].get_relay_node()->offer_event(link_event);

			get_node_array()[accept_node_id].add_next_possible_state(SOURCE_SENDING);
			log_possible_state(accept_node_id, relay_node_id, accept_node_id,
				get_node_array()[accept_node_id].get_cur_state(), SOURCE_SENDING,
				"process_ack_connection：收到接收ack，准备发送数据");
			get_node_array()[relay_node_id].add_next_possible_state(RELAY_RECEIVING);
			log_possible_state(accept_node_id, relay_node_id, relay_node_id,
				get_node_array()[relay_node_id].get_cur_state(), RELAY_RECEIVING,
				"process_ack_connection：发送ack，准备接收数");
		}

		for (int reject_node_id : ack_response.second) {
			get_node_array()[reject_node_id].add_next_possible_state(SOURCE_SEND_SYN);
			log_possible_state(reject_node_id, relay_node_id, reject_node_id,
				get_node_array()[reject_node_id].get_cur_state(), SOURCE_SEND_SYN,
				"process_ack_connection：收到拒收ack，准备再次发送syn");
		}
	}
}

void route_tcp::process_transimit_connection() {
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		if (get_node_array()[relay_node_id].get_cur_state() == RELAY_RECEIVING) {
			route_tcp_link_event* link_event = get_node_array()[relay_node_id].get_relay_node()->peek_event();
			link_event->transimit();
			int source_node_id = link_event->get_source_node_id();

			if (link_event->is_finished()) {
				get_node_array()[source_node_id].add_next_possible_state(SOURCE_LINK_RESPONSE);
				log_possible_state(source_node_id, relay_node_id, source_node_id,
					get_node_array()[source_node_id].get_cur_state(), SOURCE_LINK_RESPONSE,
					"process_transimit_connection：发送完毕，准备等待response");
				get_node_array()[relay_node_id].add_next_possible_state(RELAY_LINK_RESPONSE);
				log_possible_state(source_node_id, relay_node_id, relay_node_id,
					get_node_array()[relay_node_id].get_cur_state(), RELAY_LINK_RESPONSE,
					"process_transimit_connection：接收完毕，准备发送response");
			}
			else {
				get_node_array()[source_node_id].add_next_possible_state(SOURCE_SENDING);
				log_possible_state(source_node_id, relay_node_id, source_node_id,
					get_node_array()[source_node_id].get_cur_state(), SOURCE_SENDING,
					"process_transimit_connection：尚未传完，准备继续发送");
				get_node_array()[relay_node_id].add_next_possible_state(RELAY_RECEIVING);
				log_possible_state(source_node_id, relay_node_id, relay_node_id,
					get_node_array()[relay_node_id].get_cur_state(), RELAY_RECEIVING,
					"process_transimit_connection：尚未传完，准备继续接收");
			}
		}
	}
}

void route_tcp::process_response_connection() {
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		if (get_node_array()[relay_node_id].get_cur_state() == RELAY_LINK_RESPONSE) {
			//将link事件从relay节点弹出
			route_tcp_link_event* link_event = get_node_array()[relay_node_id].get_relay_node()->poll_event();
			if (!link_event->is_finished())throw logic_error("error");
			int source_node_id = link_event->get_source_node_id();

			if (link_event->get_is_loss()) {
				log_link_event_state(source_node_id, relay_node_id, "FAILED");

				add_failed_event(link_event);//添加到失败事件列表

				get_node_array()[source_node_id].add_next_possible_state(SOURCE_SEND_SYN);
				log_possible_state(source_node_id, relay_node_id, source_node_id,
					get_node_array()[source_node_id].get_cur_state(), SOURCE_SEND_SYN,
					"process_response_connection：丢包，准备重传");
				get_node_array()[relay_node_id].add_next_possible_state(IDLE);
				log_possible_state(source_node_id, relay_node_id, relay_node_id,
					get_node_array()[relay_node_id].get_cur_state(), IDLE,
					"process_response_connection：丢包，准备进入空闲状态");
			}
			else {
				//成功传输，将route_event转移到新的节点，并在source节点弹出
				route_tcp_route_event * route_event = get_node_array()[source_node_id].get_source_node()->poll_event()->clone();
				route_event->set_current_node_id(relay_node_id);

				get_node_array()[relay_node_id].get_source_node()->offer_event(route_event);

				log_link_event_state(source_node_id, relay_node_id, "SUCCEED");

				get_node_array()[source_node_id].add_next_possible_state(IDLE);
				log_possible_state(source_node_id, relay_node_id, source_node_id,
					get_node_array()[source_node_id].get_cur_state(), IDLE,
					"process_response_connection：成功发送，准备进入空闲状态");
				if (route_event->is_finished()) {
					add_successful_event(route_event);//添加到成功事件列表
					get_node_array()[relay_node_id].get_source_node()->poll_event();
					get_node_array()[relay_node_id].add_next_possible_state(IDLE);
					log_possible_state(source_node_id, relay_node_id, relay_node_id,
						get_node_array()[relay_node_id].get_cur_state(), IDLE,
						"process_response_connection：整条链路完毕，准备进入空闲状态");
				}
				else {
					get_node_array()[relay_node_id].add_next_possible_state(SOURCE_SEND_SYN);
					log_possible_state(source_node_id, relay_node_id, relay_node_id,
						get_node_array()[relay_node_id].get_cur_state(), SOURCE_SEND_SYN,
						"process_response_connection：尚未传输完毕，从接收节点转为发送节点");
				}
			}
		}
	}
}

void route_tcp::update_node_state() {
	for (int node_id = 0; node_id < route_tcp_node::s_node_count; node_id++) {
		get_node_array()[node_id].update_state();
	}
}