/*
* =====================================================================================
*
*       Filename:  wt.cpp
*
*    Description:  高速场景类实现
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  LN
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<fstream>
#include<iterator>
#include<vector>
#include"context.h"
#include"config.h"
#include"wt.h"
#include"vue_physics.h"


using namespace std;

default_random_engine wt::s_engine(0);

std::vector<double>* wt::m_qpsk_mi = nullptr;

void wt::set_resource() {
	ifstream in;
	if (context::get_context()->get_global_control_config()->get_platform()== Windows) {
		in.open("wt\\qpsk_mi.md");
	}
	else {
		in.open("wt/qpsk_mi.md");
	}
	m_qpsk_mi = new vector<double>();
	istream_iterator<double> in_iter(in), eof;
	m_qpsk_mi->assign(in_iter, eof);
	in.close();
}

wt::wt() {

}

wt::~wt() {

}

double wt::calculate_sinr(int t_send_vue_id, int t_receive_vue_id, int t_pattern_idx, const std::set<int>& t_sending_vue_id_set) {
	m_ploss = vue_physics::get_pl(t_send_vue_id, t_receive_vue_id);
	int subcarrier_num = context::get_context()->get_rrm_config()->get_rb_num_per_pattern() * 12;
	m_pt = pow(10, (29 - 10 * log10(subcarrier_num * 15 * 1000)) / 10);
	m_sigma = pow(10, -17.4);

	m_inter_ploss.clear();

	for (int inter_vue_id : t_sending_vue_id_set) {
		if (t_send_vue_id == inter_vue_id) continue;
		m_inter_ploss.push_back(vue_physics::get_pl(t_receive_vue_id, inter_vue_id));
	}

	//求每个子载波上的信噪比
	vector<double> sinr(subcarrier_num);//每个子载波上的信噪比，维度为nt的向量
	for (int subcarrier_idx = 0; subcarrier_idx <subcarrier_num; subcarrier_idx++) {
		double molecule = m_pt*m_ploss;
		double h_sum2 = 0;
		for (int j = 0; j < m_inter_ploss.size(); j++) {
			double weight = m_pt*m_inter_ploss[j];
			h_sum2 += weight;
		}
		double denominator = m_sigma + h_sum2;

		sinr[subcarrier_idx] = 10 * log10(molecule / denominator);
	}

	//互信息方法求有效信噪比sinreff

	double sum_mi = 0, ave_mi = 0;
	double sinreff = 0;

	for (int k = 0; k < subcarrier_num; k++) {
		sum_mi = sum_mi + get_mutual_information(*m_qpsk_mi, (int)ceil((sinr[k] + 20) * 2));
	}
	ave_mi = sum_mi / subcarrier_num;

	int snr_index = closest(*m_qpsk_mi, ave_mi);
	sinreff = 0.5*(snr_index - 40);

	return sinreff;
}


int wt::closest(std::vector<double> t_vec, double t_target) {
	int left_index = 0;
	int right_index = static_cast<int>(t_vec.size() - 1);
	double left_diff = t_vec[left_index] - t_target;
	double right_diff = t_vec[right_index] - t_target;

	while (left_index <= right_index) {
		if (right_diff <= 0) return right_index;//???
		if (left_diff >= 0) return left_index;//???

		int mid_index = left_index + ((right_index - left_index) >> 1);
		double mid_diff = t_vec[mid_index] - t_target;
		if (mid_diff == 0) return mid_index;
		else if (mid_diff < 0) {
			left_index = mid_index + 1;
			left_diff = t_vec[left_index] - t_target;
			if (abs(mid_diff) < abs(left_diff)) return mid_index;
		}
		else {
			right_index = mid_index - 1;
			right_diff = t_vec[right_index] - t_target;
			if (abs(mid_diff) < abs(right_diff)) return mid_index;
		}
	}
	return abs(t_vec[left_index] - t_target) < abs(t_vec[left_index - 1] - t_target) ? left_index : left_index - 1;//???

}

double wt::get_mutual_information(std::vector<double> t_vec, int t_index) {
	if (t_index < 0) return t_vec[0];
	if (t_index >= (int)t_vec.size()) return t_vec[t_vec.size() - 1];
	return t_vec[t_index];
}