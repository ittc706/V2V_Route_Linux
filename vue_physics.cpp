/*
* =====================================================================================
*
*       Filename:  vue_physics.cpp
*
*    Description:  车辆类的物理层部分实现
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  HCF
*            LIB:  ITTC
*
* =====================================================================================
*/

#include"vue.h"
#include"vue_physics.h"
#include"context.h"
#include"gtt.h"
#include"gtt_urban.h"
#include"gtt_highspeed.h"
#include"config.h"
#include"imta.h"
#include"function.h"

using namespace std;

int vue_physics::s_vue_count = 0;

vector<vector<double>> vue_physics::s_pl_all(0);

vector<vector<double>> vue_physics::s_distance_all(0);

int vue_physics::get_vue_num() {
	return s_vue_count;
}

void vue_physics::set_pl(int i, int j, double t_pl) {
	if (i < j) {
		s_pl_all[i][j] = t_pl;
	}
	else {
		s_pl_all[j][i] = t_pl;
	}
}

double vue_physics::get_pl(int i, int j) {
	if (i < j) {
		return s_pl_all[i][j];
	}
	else {
		return s_pl_all[j][i];
	}
}

void vue_physics::set_distance(int i, int j, double t_distance) {
	if (i < j) {
		s_distance_all[i][j] = t_distance;
	}
	else {
		s_distance_all[j][i] = t_distance;
	}
}

double vue_physics::get_distance(int i, int j) {
	if (i < j) {
		return s_distance_all[i][j];
	}
	else {
		return s_distance_all[j][i];
	}
}

vue_physics::vue_physics() {

}

vue_physics::~vue_physics() {
	
}

int vue_physics::get_congestion_level() {
	return m_congestion_level;
}

void vue_physics::set_slot_time_idx(int t_slot_time_idx) {
	this->m_slot_time_idx = t_slot_time_idx;
}

int vue_physics::get_slot_time_idx() {
	return m_slot_time_idx;
}

void vue_physics::update_location_highspeed() {
	auto p = (gtt_highspeed*)context::get_context()->get_gtt();
	//get_freshtime()的单位是TTI，这里转换成s
	double freshtime_second = static_cast<double>(p->get_precise_config()->get_freshtime()) / 1000.0;
	if (m_vangle == 0)
	{
		if ((m_absx + freshtime_second*m_speed)>(p->get_precise_config()->get_road_length() / 2))
		{
			m_absx = (m_absx + freshtime_second*m_speed) - p->get_precise_config()->get_road_length();
			m_relx = m_absx;
		}
		else
		{
			m_absx = m_absx + freshtime_second*m_speed;
			m_relx = m_absx;
		}
	}
	else
	{
		if ((m_absx - freshtime_second*m_speed)<(-p->get_precise_config()->get_road_length() / 2))
		{
			m_absx = m_absx - freshtime_second*m_speed + p->get_precise_config()->get_road_length();
			m_relx = m_absx;
		}
		else
		{
			m_absx = m_absx - freshtime_second*m_speed;
			m_relx = m_absx;
		}
	}
}

void vue_physics::update_location_urban() {
	auto p = (gtt_urban*)context::get_context()->get_gtt();
	//get_freshtime()的单位是TTI，这里转换成s
	double freshtime_second = static_cast<double>(p->get_precise_config()->get_freshtime()) / 1000.0;
	bool RoadChangeFlag = false;
	int temp;
	if (m_vangle == 90) {//left
		if ((m_rely + freshtime_second*m_speed) > (p->get_precise_config()->get_road_length_ew() / 2)) {//top left
			temp = rand() % 4;
			if (temp == 0) {//turn left
				RoadChangeFlag = true;
				m_relx = p->get_precise_config()->get_road_length_sn() / 2 - (m_rely + freshtime_second*m_speed - p->get_precise_config()->get_road_length_ew() / 2);
				m_rely = -(p->get_precise_config()->get_road_length_ew() + p->get_precise_config()->get_road_width()) / 2;
				m_road_id = p->get_precise_config()->get_wrap_around_road()[m_road_id][6];
				m_vangle = -180;
			}
			else if (temp == 2) {//turn right
				m_relx = (m_rely + freshtime_second*m_speed - p->get_precise_config()->get_road_length_ew() / 2) - p->get_precise_config()->get_road_length_sn() / 2;
				m_rely = (p->get_precise_config()->get_road_length_ew() + p->get_precise_config()->get_road_width()) / 2;
				m_vangle = 0;
			}
			else {//go straight
				RoadChangeFlag = true;
				m_rely = (m_rely + freshtime_second*m_speed - p->get_precise_config()->get_road_length_ew() / 2) - p->get_precise_config()->get_road_length_ew() / 2;
				m_road_id = p->get_precise_config()->get_wrap_around_road()[m_road_id][7];
			}
		}
		else {
			m_rely = m_rely + freshtime_second*m_speed;
		}
	}

	else if (m_vangle == 0) {//top
		if ((m_relx + freshtime_second*m_speed) > (p->get_precise_config()->get_road_length_sn() / 2)) {//top right
			temp = rand() % 4;
			if (temp == 0) {//turn left
				RoadChangeFlag = true;
				m_rely = (m_relx + freshtime_second*m_speed - p->get_precise_config()->get_road_length_sn() / 2) - p->get_precise_config()->get_road_length_ew() / 2;
				m_relx = -(p->get_precise_config()->get_road_length_sn() + p->get_precise_config()->get_road_width()) / 2;
				m_road_id = p->get_precise_config()->get_wrap_around_road()[m_road_id][8];
				m_vangle = 90;
			}
			else if (temp == 2) {//turn right
				m_rely = p->get_precise_config()->get_road_length_ew() / 2 - (m_relx + freshtime_second*m_speed - p->get_precise_config()->get_road_length_sn() / 2);
				m_relx = (p->get_precise_config()->get_road_length_sn() + p->get_precise_config()->get_road_width()) / 2;
				m_vangle = -90;
			}
			else {//go straight
				RoadChangeFlag = true;
				m_relx = (m_relx + freshtime_second*m_speed - p->get_precise_config()->get_road_length_sn() / 2) - p->get_precise_config()->get_road_length_sn() / 2;
				m_road_id = p->get_precise_config()->get_wrap_around_road()[m_road_id][1];
			}
		}
		else {
			m_relx = m_relx + freshtime_second*m_speed;
		}
	}

	else if (m_vangle == -90) {//right
		if ((m_rely - freshtime_second*m_speed) < -(p->get_precise_config()->get_road_length_ew() / 2)) {//bottom right
			temp = rand() % 4;
			if (temp == 0) {//turn left
				RoadChangeFlag = true;
				m_relx = (-p->get_precise_config()->get_road_length_ew() / 2 - (m_rely - freshtime_second*m_speed)) - p->get_precise_config()->get_road_length_sn() / 2;
				m_rely = (p->get_precise_config()->get_road_length_ew() + p->get_precise_config()->get_road_width()) / 2;
				m_road_id = p->get_precise_config()->get_wrap_around_road()[m_road_id][2];
				m_vangle = 0;
			}
			else if (temp == 2) {//turn right
				m_relx = p->get_precise_config()->get_road_length_sn() / 2 - (-p->get_precise_config()->get_road_length_ew() / 2 - (m_rely - freshtime_second*m_speed));
				m_rely = -(p->get_precise_config()->get_road_length_ew() + p->get_precise_config()->get_road_width()) / 2;
				m_vangle = -180;
			}
			else {//go straight
				RoadChangeFlag = true;
				m_rely = p->get_precise_config()->get_road_length_ew() / 2 - (-p->get_precise_config()->get_road_length_ew() / 2 - (m_rely - freshtime_second*m_speed));
				m_road_id = p->get_precise_config()->get_wrap_around_road()[m_road_id][3];
			}
		}
		else {
			m_rely = m_rely - freshtime_second*m_speed;
		}
	}

	else {//bottom
		if ((m_relx - freshtime_second*m_speed) < -(p->get_precise_config()->get_road_length_sn() / 2)) {//bottom left
			temp = rand() % 4;
			if (temp == 0) {//turn left
				RoadChangeFlag = true;
				m_rely = p->get_precise_config()->get_road_length_ew() / 2 - (-p->get_precise_config()->get_road_length_sn() / 2 - (m_relx - freshtime_second*m_speed));
				m_relx = (p->get_precise_config()->get_road_length_sn() + p->get_precise_config()->get_road_width()) / 2;
				m_road_id = p->get_precise_config()->get_wrap_around_road()[m_road_id][4];
				m_vangle = -90;
			}
			else if (temp == 2) {//turn right
				m_rely = (-p->get_precise_config()->get_road_length_sn() / 2 - (m_relx - freshtime_second*m_speed)) - p->get_precise_config()->get_road_length_ew() / 2;
				m_relx = -(p->get_precise_config()->get_road_length_sn() + p->get_precise_config()->get_road_width()) / 2;
				m_vangle = 90;
			}
			else {//go straight
				RoadChangeFlag = true;
				m_relx = p->get_precise_config()->get_road_length_sn() / 2 - (-p->get_precise_config()->get_road_length_sn() / 2 - (m_relx - freshtime_second*m_speed));
				m_road_id = p->get_precise_config()->get_wrap_around_road()[m_road_id][5];
			}
		}
		else {
			m_relx = m_relx - freshtime_second*m_speed;
		}
	}
	m_absx = p->get_precise_config()->get_road_topo_ratio()[m_road_id * 2 + 0] * (p->get_precise_config()->get_road_length_sn() + 2 * p->get_precise_config()->get_road_width()) + m_relx;
	m_absy = p->get_precise_config()->get_road_topo_ratio()[m_road_id * 2 + 1] * (p->get_precise_config()->get_road_length_ew() + 2 * p->get_precise_config()->get_road_width()) + m_rely;

}
void vue_physics::set_superior_level(vue* t_superior_level) {
	m_superior_level = t_superior_level;
}

vue* vue_physics::get_superior_level() {
	return m_superior_level;
}

int vue_physics::get_vue_id() {
	return m_vue_id;
}