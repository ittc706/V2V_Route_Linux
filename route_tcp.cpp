#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include"route_tcp.h"
#include"config.h"
#include"gtt.h"
#include"wt.h"
#include"vue.h"
#include"vue_physics.h"
#include"function.h"
#include"reflect/context.h"
#include"time_stamp.h"

using namespace std;

int route_tcp_route_event::s_event_count = 0;

std::string route_tcp_route_event::to_string() {
	stringstream ss;
	for (int i = 0; i < m_through_node_id_vec.size(); i++) {
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

	double sinr = ((wt*)context::get_context()->get_bean("wt"))->calculate_sinr(
		get_source_node_id(),
		get_destination_node_id(),
		get_pattern_idx(),
		route_tcp_node::get_node_id_set(get_pattern_idx()));

	if (sinr < ((rrm_config*)context::get_context()->get_bean("rrm_config"))->get_drop_sinr_boundary() - 100) {
		m_is_loss = true;
	}
}

int route_tcp_node::s_node_count = 0;

default_random_engine route_tcp_node::s_engine;

std::vector<std::set<int>> route_tcp_node::s_node_id_per_pattern;

const std::set<int>& route_tcp_node::get_node_id_set(int t_pattern_idx) {
	return s_node_id_per_pattern[t_pattern_idx];
}

route_tcp_node::route_tcp_node() {
	rrm_config* __rrm_config = (rrm_config*)context::get_context()->get_bean("rrm_config");
	m_pattern_state = vector<pair<route_tcp_pattern_state, route_tcp_link_event*>>(
		__rrm_config->get_pattern_num(),
		pair<route_tcp_pattern_state, route_tcp_link_event*>(IDLE, nullptr)
		);

	m_select_cache = pair<int, int>(-1, -1);
	m_last_round_request_per_pattern = vector<vector<int>>(
		__rrm_config->get_pattern_num()
		);
	m_syn_request_per_pattern = vector<vector<int>>(
		__rrm_config->get_pattern_num()
		);

	m_next_round_link_event = vector<route_tcp_link_event*>(
		__rrm_config->get_pattern_num(),
		nullptr
		);
}


pair<int, int> route_tcp_node::select_relay_information() {
	pair<int, int> res = make_pair<int, int>(-1, -1);

	//先挑选路由车辆id
	//<Warn>可以增加其他算法
	int final_destination_node_id = peek_send_event_queue()->get_final_destination_node_id();

	double min_distance = (numeric_limits<double>::max)();
	for (int near_node_id : m_adjacent_list) {
		double cur_distance = vue_physics::get_distance(near_node_id, final_destination_node_id);
		if (cur_distance< min_distance) {
			min_distance = cur_distance;
			res.first = near_node_id;
		}
	}

	//挑选频段，必须在自身未占用的频段上挑选
	vector<int> candidate;
	for (int pattern_idx = 0; pattern_idx < m_pattern_state.size(); pattern_idx++) {
		if (m_pattern_state[pattern_idx].first == IDLE) {
			candidate.push_back(pattern_idx);
		}

	}
	if (candidate.size() != 0) {
		//在未占用的频段上随机挑选一个
		//<Warn>可以增加其他算法
		uniform_int_distribution<int> u(0, static_cast<int>(candidate.size()) - 1);
		res.second = candidate[u(s_engine)];
	}

	return res;
}

default_random_engine route_tcp::s_engine;

ofstream route_tcp::s_logger_pattern;
ofstream route_tcp::s_logger_link;
ofstream route_tcp::s_logger_event;

void route_tcp::log_node_pattern(int t_source_node_id,
	int t_relay_node_id,
	int t_cur_node_id,
	int t_pattern_idx,
	route_tcp_pattern_state t_from_pattern_state,
	route_tcp_pattern_state t_to_pattern_state,
	string t_description) {
	v2x_time* __time = (v2x_time*)context::get_context()->get_bean("time");
	s_logger_pattern << "TTI[" << left << setw(3) << __time->get_tti() << "] - ";
	s_logger_pattern << "link[" << left << setw(3) << t_source_node_id << ", ";
	s_logger_pattern << left << setw(3) << t_relay_node_id << "] - ";
	s_logger_pattern << "node[" << left << setw(3) << t_cur_node_id << "] - ";
	s_logger_pattern << "pattern[" << t_pattern_idx << "] - ";
	s_logger_pattern << "[" << left << setw(15) << pattern_state_to_string(t_from_pattern_state) << " -> " << left << setw(15) << pattern_state_to_string(t_to_pattern_state) << "] - ";
	s_logger_pattern << t_description << endl;
}

string route_tcp::pattern_state_to_string(route_tcp_pattern_state t_pattern_state) {
	switch (t_pattern_state) {
	case IDLE:
		return "IDLE";
	case TO_BE_SEND:
		return "TO_BE_SEND";
	case TO_BE_RECEIVE:
		return "TO_BE_RECEIVE";
	case SENDING:
		return "SENDING";
	case RECEIVING:
		return "RECEIVING";
	default:
		throw logic_error("error");
	}
}

void route_tcp::log_event(int t_origin_node_id, int t_fianl_destination_node_id) {
	v2x_time* __time = (v2x_time*)context::get_context()->get_bean("time");
	s_logger_event << "TTI[" << left << setw(3) << __time->get_tti() << "] - ";
	s_logger_event << "trigger[" << left << setw(3) << t_origin_node_id << ", ";
	s_logger_event << left << setw(3) << t_fianl_destination_node_id << "]" << endl;

}

void route_tcp::log_link(int t_source_node_id, int t_relay_node_id, std::string t_description) {
	v2x_time* __time = (v2x_time*)context::get_context()->get_bean("time");
	s_logger_link << "TTI[" << left << setw(3) << __time->get_tti() << "] - ";
	s_logger_link << "link[" << left << setw(3) << t_source_node_id << ", ";
	s_logger_link << left << setw(3) << t_relay_node_id << "] - ";
	s_logger_link << "{" << t_description << "}" << endl;
}


void route_tcp::initialize() {
	context* __context = context::get_context();
	gtt* ggg = get_gtt();
	int vue_num = get_gtt()->get_vue_num();
	m_node_array = new route_tcp_node[vue_num];

	s_logger_pattern.open("log/route_tcp_pattern_log.txt");
	s_logger_link.open("log/route_tcp_link_log.txt");
	s_logger_event.open("log/route_tcp_event_log.txt");

	route_tcp_node::s_node_id_per_pattern = vector<set<int>>(get_rrm_config()->get_pattern_num());
}

void route_tcp::process_per_tti() {
	//事件触发
	event_trigger();

	//在tti开始时，刷新tobe数据结构
	update_tobe();

	//发送syn
	send_syn();

	//发送ack
	send_ack();

	//进行接收(收发两端，仅以收端作为处理点，发端在收端处理的同时一并处理)
	receive_data();
}

void route_tcp::update_route_table_from_physics_level() {
	//清除之前的邻接表
	for (int node_id = 0; node_id < route_tcp_node::s_node_count; node_id++) {
		get_node_array()[node_id].m_adjacent_list.clear();
	}

	//<Warn>:暂时改为根据距离确定邻接表
	context* __context = context::get_context();
	int vue_num = get_gtt()->get_vue_num();
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
	double trigger_rate = get_tmc_config()->get_trigger_rate();

	uniform_real_distribution<double> u_rate(0, 1);
	uniform_int_distribution<int> u_node_id(0, route_tcp_node::s_node_count - 1);

	for (int origin_source_node_id = 0; origin_source_node_id < route_tcp_node::s_node_count; origin_source_node_id++) {
		if (u_rate(s_engine) < trigger_rate) {

			int final_destination_node_id = origin_source_node_id;
			while (final_destination_node_id == origin_source_node_id) {
				final_destination_node_id = u_node_id(s_engine);
			}
			get_node_array()[origin_source_node_id].offer_send_event_queue(
				new route_tcp_route_event(origin_source_node_id, final_destination_node_id)
				);
			log_event(origin_source_node_id, final_destination_node_id);
		}
	}
}



void route_tcp::update_tobe() {
	//将TO_BE_SEND/RECEIVE转换为SENDING/RECEIVING，同时将link_event从m_next_round_link_event转移到m_pattern_state中
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		route_tcp_node& relay_node = get_node_array()[relay_node_id];
		for (int pattern_idx = 0; pattern_idx < get_rrm_config()->get_pattern_num(); pattern_idx++) {
			if (relay_node.m_next_round_link_event[pattern_idx] != nullptr) {

				route_tcp_pattern_state temp_relay_state = relay_node.m_pattern_state[pattern_idx].first;
				if (relay_node.m_pattern_state[pattern_idx].first != TO_BE_RECEIVE) throw logic_error("error");
				relay_node.m_pattern_state[pattern_idx].first = RECEIVING;
				if (relay_node.m_pattern_state[pattern_idx].second != nullptr)throw logic_error("error");

				relay_node.m_pattern_state[pattern_idx].second = relay_node.m_next_round_link_event[pattern_idx];
				relay_node.m_next_round_link_event[pattern_idx] = nullptr;

				int source_node_id = relay_node.m_pattern_state[pattern_idx].second->get_source_node_id();
				route_tcp_node& source_node = get_node_array()[source_node_id];

				route_tcp_pattern_state temp_source_state = source_node.m_pattern_state[pattern_idx].first;
				if (source_node.m_pattern_state[pattern_idx].first != TO_BE_SEND) throw logic_error("error");
				source_node.m_pattern_state[pattern_idx].first = SENDING;
				if (source_node.m_pattern_state[pattern_idx].second != nullptr) throw logic_error("error");

				//维护干扰列表，在传输完毕后，将其移除
				if (route_tcp_node::s_node_id_per_pattern[pattern_idx].find(source_node_id) != route_tcp_node::s_node_id_per_pattern[pattern_idx].end()) throw logic_error("error");
				route_tcp_node::s_node_id_per_pattern[pattern_idx].insert(source_node_id);

				log_node_pattern(
					source_node_id,
					relay_node_id,
					source_node_id,
					pattern_idx,
					temp_source_state,
					source_node.m_pattern_state[pattern_idx].first,
					""
					);

				log_node_pattern(
					source_node_id,
					relay_node_id,
					relay_node_id,
					pattern_idx,
					temp_relay_state,
					relay_node.m_pattern_state[pattern_idx].first,
					""
					);
			}
		}
	}

	//将上一tti收到的syn挪到m_last_round_request_per_pattern中，用于本次ack应答
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		route_tcp_node& relay_node = get_node_array()[relay_node_id];
		relay_node.m_last_round_request_per_pattern.swap(relay_node.m_syn_request_per_pattern);
		for (int pattern_idx = 0; pattern_idx < relay_node.m_syn_request_per_pattern.size(); pattern_idx++) {
			relay_node.m_syn_request_per_pattern[pattern_idx].clear();
		}
	}
}

void route_tcp::send_syn() {
	for (int source_node_id = 0; source_node_id < route_tcp_node::s_node_count; source_node_id++) {
		route_tcp_node& source_node = get_node_array()[source_node_id];
		//当前车辆待发送事件列表为空，跳过即可
		if (source_node.is_send_event_queue_empty()) continue;

		//已经发送过syn了，目前正在等待ack或者正在等待数据传输
		if (source_node.is_already_send_syn())continue;

		//选择中继车辆以及频段
		pair<int, int> select_res = source_node.select_relay_information();
		if (select_res.first == -1 || select_res.second == -1) {
			continue;//没有可用中继节点，或者没有可用频段
		}

		int relay_node_id = select_res.first;
		int pattern_idx = select_res.second;

		//将选择结果缓存起来,否则下个tti该节点仍会继续发送syn，与上面的continue呼应
		source_node.m_select_cache = select_res;

		//在该频段上进行占位，避免在ack中被重用。否则发送频段可能和其他节点发来的syn占用同一频段
		route_tcp_pattern_state temp_source_state = source_node.m_pattern_state[pattern_idx].first;
		if (source_node.m_pattern_state[pattern_idx].first != IDLE)throw logic_error("error");
		source_node.m_pattern_state[pattern_idx].first = TO_BE_SEND;
		if (source_node.m_pattern_state[pattern_idx].second != nullptr)  throw logic_error("error");

		log_node_pattern(
			source_node_id,
			relay_node_id,
			source_node_id,
			pattern_idx,
			temp_source_state,
			source_node.m_pattern_state[pattern_idx].first,
			""
			);

		route_tcp_node& relay_node = get_node_array()[relay_node_id];
		relay_node.add_syn_request(pattern_idx, source_node_id);
	}
}

void route_tcp::send_ack() {
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		route_tcp_node& relay_node = get_node_array()[relay_node_id];
		for (int pattern_idx = 0; pattern_idx < get_rrm_config()->get_pattern_num(); pattern_idx++) {
			//该pattern下，没有syn请求，跳过即可
			if (relay_node.m_last_round_request_per_pattern[pattern_idx].size() == 0) continue;

			if (relay_node.m_pattern_state[pattern_idx].first != IDLE) {
				//目前该频段非IDLE，说明正在发送或者正在接收
				//必须拒绝所有syn请求
				for (int rejected_source_node_id : relay_node.m_last_round_request_per_pattern[pattern_idx]) {
					route_tcp_node& rejected_source_node = get_node_array()[rejected_source_node_id];
					rejected_source_node.reset_syn_state();//清空缓存，好让该节点下一时刻继续发送syn请求

					route_tcp_pattern_state temp_rejected_source_state = rejected_source_node.m_pattern_state[pattern_idx].first;
					if (rejected_source_node.m_pattern_state[pattern_idx].first != TO_BE_SEND)throw logic_error("error");
					rejected_source_node.m_pattern_state[pattern_idx].first = IDLE;
					if (rejected_source_node.m_pattern_state[pattern_idx].second != nullptr)throw logic_error("error");

					log_node_pattern(
						rejected_source_node_id,
						relay_node_id,
						rejected_source_node_id,
						pattern_idx,
						temp_rejected_source_state,
						rejected_source_node.m_pattern_state[pattern_idx].first,
						""
						);
				}
			}
			else {
				/*首先选择一个节点响应ack请求*/
				uniform_int_distribution<int> u(0, static_cast<int>(relay_node.m_last_round_request_per_pattern[pattern_idx].size()) - 1);
				int selected_source_node_id = relay_node.m_last_round_request_per_pattern[pattern_idx][u(s_engine)];

				route_tcp_node& selected_source_node = get_node_array()[selected_source_node_id];
				route_tcp_route_event* route_event = selected_source_node.peek_send_event_queue();

				//创建链路事件
				route_tcp_link_event* link_event = new route_tcp_link_event(
					selected_source_node_id, relay_node_id, pattern_idx, route_event->get_package_num()
					);
				//添加到待发列表中，不直接添加到发送列表是避免当前tti就进行传输
				if (relay_node.m_next_round_link_event[pattern_idx] != nullptr) throw logic_error("error");
				relay_node.m_next_round_link_event[pattern_idx] = link_event;

				/*source节点已经在send_syn中将m_pattern_state置为TO_BE_SEND*/

				//维护接收节点的pattern状态
				route_tcp_pattern_state temp_relay_state = relay_node.m_pattern_state[pattern_idx].first;

				if (relay_node.m_pattern_state[pattern_idx].first != IDLE)  throw logic_error("error");
				relay_node.m_pattern_state[pattern_idx].first = TO_BE_RECEIVE;
				if (relay_node.m_pattern_state[pattern_idx].second != nullptr) throw logic_error("error");

				log_node_pattern(
					selected_source_node_id,
					relay_node_id,
					relay_node_id,
					pattern_idx,
					temp_relay_state,
					relay_node.m_pattern_state[pattern_idx].first,
					""
					);

				/*selected_source_node不要清除select_cache，避免在传输过程中，又请求其他车辆发送*/

				/*然后拒绝其他节点的syn请求*/
				for (int rejected_source_node_id : relay_node.m_last_round_request_per_pattern[pattern_idx]) {
					if (rejected_source_node_id == selected_source_node_id)continue;
					route_tcp_node& rejected_source_node = get_node_array()[rejected_source_node_id];
					rejected_source_node.reset_syn_state();//清空缓存，好让该节点下一时刻继续发送syn请求

					route_tcp_pattern_state temp_rejected_source_state = rejected_source_node.m_pattern_state[pattern_idx].first;
					if (rejected_source_node.m_pattern_state[pattern_idx].first != TO_BE_SEND)throw logic_error("error");
					rejected_source_node.m_pattern_state[pattern_idx].first = IDLE;
					if (rejected_source_node.m_pattern_state[pattern_idx].second != nullptr)throw logic_error("error");

					log_node_pattern(
						rejected_source_node_id,
						relay_node_id,
						rejected_source_node_id,
						pattern_idx,
						temp_rejected_source_state,
						rejected_source_node.m_pattern_state[pattern_idx].first,
						""
						);
				}
			}
		}
	}
}

void route_tcp::receive_data() {
	for (int relay_node_id = 0; relay_node_id < route_tcp_node::s_node_count; relay_node_id++) {
		route_tcp_node& relay_node = get_node_array()[relay_node_id];
		for (int pattern_idx = 0; pattern_idx < get_rrm_config()->get_pattern_num(); pattern_idx++) {
			if (relay_node.m_pattern_state[pattern_idx].first == RECEIVING) {
				route_tcp_link_event* link_event = relay_node.m_pattern_state[pattern_idx].second;

				int source_node_id = link_event->get_source_node_id();
				route_tcp_node& source_node = get_node_array()[source_node_id];
				if (source_node.m_pattern_state[pattern_idx].first != SENDING)throw logic_error("error");
				if (source_node.m_pattern_state[pattern_idx].second != nullptr)throw logic_error("error");

				link_event->transimit();

				if (link_event->is_finished()) {
					//维护收发节点的m_pattern状态
					route_tcp_pattern_state temp_source_state = source_node.m_pattern_state[pattern_idx].first;
					source_node.m_pattern_state[pattern_idx].first = IDLE;

					route_tcp_pattern_state temp_relay_state = relay_node.m_pattern_state[pattern_idx].first;
					relay_node.m_pattern_state[pattern_idx].first = IDLE;
					relay_node.m_pattern_state[pattern_idx].second = nullptr;//<Warn>这里就直接把link_event删掉了

					if (route_tcp_node::s_node_id_per_pattern[pattern_idx].find(source_node_id) == route_tcp_node::s_node_id_per_pattern[pattern_idx].end()) throw logic_error("error");
					route_tcp_node::s_node_id_per_pattern[pattern_idx].erase(source_node_id);

					log_node_pattern(
						source_node_id,
						relay_node_id,
						source_node_id,
						pattern_idx,
						temp_source_state,
						source_node.m_pattern_state[pattern_idx].first,
						""
						);

					log_node_pattern(
						source_node_id,
						relay_node_id,
						relay_node_id,
						pattern_idx,
						temp_relay_state,
						relay_node.m_pattern_state[pattern_idx].first,
						""
						);

					if (link_event->get_is_loss()) {
						//丢包了
						log_link(source_node_id, relay_node_id, "FAILED");

						add_failed_event(link_event);
						//重置后重传
						source_node.reset_syn_state();
					}
					else {
						//没有丢包
						log_link(source_node_id, relay_node_id, "SUCCESSFUL");

						relay_node.offer_send_event_queue(source_node.poll_send_event_queue());
						relay_node.peek_send_event_queue()->set_current_node_id(relay_node_id);
						if (relay_node.peek_send_event_queue()->is_finished()) {
							add_successful_event(relay_node.poll_send_event_queue());
						}

						delete link_event;
						//重置
						source_node.reset_syn_state();
					}
				}
			}
		}
	}
}
