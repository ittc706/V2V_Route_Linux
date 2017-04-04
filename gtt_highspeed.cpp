/*
* =====================================================================================
*
*       Filename:  gtt_highspeed.cpp
*
*    Description:  高速场景类实现
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  WYB
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<fstream>
#include"context.h"
#include"route.h"
#include"config.h"
#include"gtt_highspeed.h"
#include"vue.h"
#include"vue_physics.h"
#include"imta.h"
#include"function.h"
#include<memory.h>



using namespace std;

void gtt_highspeed::initialize() {
	gtt_highspeed_config* __config = get_precise_config();
	int* m_pupr = new int[__config->get_road_num()];//每条路上的车辆数
	double* TotalTime = new double[__config->get_road_num()];//每条道路初始泊松撒点过程中所有车辆都已撒进区域内所用的总时间
	std::list<double>* possion = new std::list<double>[__config->get_road_num()];//每条道路初始泊松撒点的车辆到达时间间隔list，单位s

	srand((unsigned)time(0));

	//生成负指数分布的车辆到达间隔
	int tempVeUENum = 0;
	double lambda = 1 / 2.5;//均值为1/lambda，依照协议车辆到达时间间隔的均值为2.5s
	for (int roadId = 0; roadId != __config->get_road_num(); roadId++) {
		TotalTime[roadId] = 0;
		while (TotalTime[roadId] * (__config->get_speed() / 3.6) < __config->get_road_length()) {
			double pV = 0.0;
			while (true)
			{
				pV = (double)rand() / (double)RAND_MAX;
				if (pV != 1)
				{
					break;
				}
			}
			pV = (-1.0 / lambda)*log(1 - pV);
			possion[roadId].push_back(pV);
			TotalTime[roadId] += pV;
			double check = TotalTime[roadId];
		}
		m_pupr[roadId] = static_cast<int>(possion[roadId].size());//完成当前道路下总车辆数的赋值
		tempVeUENum += m_pupr[roadId];
	}

	//进行车辆的撒点
	context::get_context()->set_vue_array(new vue[tempVeUENum]);
	int vue_id = 0;

	/*ofstream vue_coordinate;

	if (context::get_context()->get_global_control_config()->get_platform() == Windows) {
		vue_coordinate.open("log\\vue_coordinate.txt");
	}
	else {
		vue_coordinate.open("log/vue_coordinate.txt");
	}*/

	for (int roadId = 0; roadId != __config->get_road_num(); roadId++) {
		for (int uprIdx = 0; uprIdx != m_pupr[roadId]; uprIdx++) {
			auto p = context::get_context()->get_vue_array()[vue_id++].get_physics_level();
			p->m_speed = __config->get_speed()/3.6;
		    p->m_absx = -1732 + (TotalTime[roadId] - possion[roadId].back())*(p->m_speed);
			p->m_absy = __config->get_road_topo_ratio()[roadId * 2 + 1]* __config->get_road_width();
			//将撒点后的坐标输出到txt文件
			//vue_coordinate << p->m_absx << " ";
			//vue_coordinate << p->m_absy << " ";
			//vue_coordinate << endl;

			TotalTime[roadId] = TotalTime[roadId] - possion[roadId].back();
			possion[roadId].pop_back();
		
			//初始化pattern占用情况的数组全为false，即未被占用状态
			p->m_pattern_occupied = new bool[context::get_context()->get_rrm_config()->get_pattern_num()];
			memset(p->m_pattern_occupied, false, sizeof(p->m_pattern_occupied));

			//根据是否采用时分的资源分配算法决定是否维护m_slot_time_idx,即当前车辆能发送数据的TTI
			int granularity = context::get_context()->get_rrm_config()->get_time_division_granularity();
			if (granularity == 2) {
				double zone_length = 346.41;
				int zone_idx = (int)abs((p->m_absx - (-1732.0f)) / 346.4f);//0到9
				if ((zone_idx + 1) % 2 == 1) {
					p->m_slot_time_idx = 0;//若当前区域采用odd subframe，则赋值0
				}
				else {
					p->m_slot_time_idx = 1;//若当前区域采用even subframe，则赋值1
				}
			}
		}
	}

	//vue_coordinate.close();

	memory_clean::safe_delete(m_pupr, true);
	memory_clean::safe_delete(TotalTime, true);
	memory_clean::safe_delete(possion, true);

	vue_physics::s_pl_all.assign(get_vue_num(), std::vector<double>(get_vue_num(), 0));
	vue_physics::s_distance_all.assign(get_vue_num(), std::vector<double>(get_vue_num(), 0));
}

int gtt_highspeed::get_vue_num() {
	return vue_physics::get_vue_num();
}

void gtt_highspeed::fresh_location() {
	//<Warn>:将信道刷新时间和位置刷新时间分开
	if (context::get_context()->get_tti() % get_precise_config()->get_freshtime() != 0) {
		return;
	}

	for (int vue_id = 0; vue_id < get_vue_num(); vue_id++) {
		context::get_context()->get_vue_array()[vue_id].get_physics_level()->update_location_highspeed();

		//每次更新车辆位置时重新判断车辆所在的zone_idx
		auto p = context::get_context()->get_vue_array()[vue_id].get_physics_level();
		int granularity = context::get_context()->get_rrm_config()->get_time_division_granularity();
		if (granularity == 2) {
			double zone_length = 346.41;
			int zone_idx = (int)abs((p->m_absx - (-1732.0f)) / 346.4f);//0到9
			if ((zone_idx + 1) % 2 == 1) {
				p->m_slot_time_idx = 0;//若当前区域采用odd subframe，则赋值0
			}
			else {
				p->m_slot_time_idx = 1;//若当前区域采用even subframe，则赋值1
			}
		}
	}

	for (int vue_id1 = 0; vue_id1 < get_vue_num(); vue_id1++) {
		for (int vue_id2 = 0; vue_id2 < vue_id1; vue_id2++) {
			auto vuei = context::get_context()->get_vue_array()[vue_id1].get_physics_level();
			auto vuej = context::get_context()->get_vue_array()[vue_id2].get_physics_level();
			vue_physics::set_distance(vue_id2, vue_id1, sqrt(pow((vuei->m_absx - vuej->m_absx), 2.0f) + pow((vuei->m_absy - vuej->m_absy), 2.0f)));
			calculate_pl(vue_id1, vue_id2);
		}
	}

	context::get_context()->get_route()->update_route_table_from_physics_level();
}


void gtt_highspeed::calculate_pl(int t_vue_id1, int t_vue_id2) {

	location _location;
	_location.eNBAntH = 5;
	_location.VeUEAntH = 1.5;
	_location.RSUAntH = 5;
	_location.locationType = None;
	_location.distance = 0;
	_location.distance1 = 0;
	_location.distance2 = 0;

	antenna _antenna;
	_antenna.antGain = 3;
	_antenna.byTxAntNum = 1;
	_antenna.byRxAntNum = 2;

	imta* __imta = new imta();

	auto vuei = context::get_context()->get_vue_array()[t_vue_id1].get_physics_level();
	auto vuej = context::get_context()->get_vue_array()[t_vue_id2].get_physics_level();

	_location.locationType = Los;
	_location.manhattan = false;

	double angle = 0;
	_location.distance = vue_physics::get_distance(t_vue_id1, t_vue_id2);

	angle = atan2(vuei->m_absy - vuej->m_absy, vuei->m_absx - vuej->m_absx) / imta::s_DEGREE_TO_PI;

	imta::randomGaussian(_location.posCor, 5, 0.0f, 1.0f);//产生高斯随机数，为后面信道系数使用

	double m_FantennaAnglei;
	double m_FantennaAnglej;

	imta::randomUniform(&m_FantennaAnglei, 1, 180.0f, -180.0f, false);
	imta::randomUniform(&m_FantennaAnglej, 1, 180.0f, -180.0f, false);

	_antenna.TxAngle = angle - m_FantennaAnglei;
	_antenna.RxAngle = angle - m_FantennaAnglej;
	_antenna.TxSlantAngle = new double[_antenna.byTxAntNum];
	_antenna.TxAntSpacing = new double[_antenna.byTxAntNum];
	_antenna.RxSlantAngle = new double[_antenna.byRxAntNum];
	_antenna.RxAntSpacing = new double[_antenna.byRxAntNum];
	_antenna.TxSlantAngle[0] = 90.0f;
	_antenna.TxAntSpacing[0] = 0.0f;
	_antenna.RxSlantAngle[0] = 90.0f;
	_antenna.RxSlantAngle[1] = 90.0f;
	_antenna.RxAntSpacing[0] = 0.0f;
	_antenna.RxAntSpacing[1] = 0.5f;

	double t_Pl = 0;

	__imta->build(&t_Pl, imta::s_FC, _location, _antenna, vuei->m_speed, vuej->m_speed, vuei->m_vangle, vuej->m_vangle);//计算了结果代入信道模型计算UE之间信道系数

	vue_physics::set_pl(t_vue_id1, t_vue_id2, t_Pl);

	memory_clean::safe_delete(_antenna.TxSlantAngle, true);
	memory_clean::safe_delete(_antenna.TxAntSpacing, true);
	memory_clean::safe_delete(_antenna.RxSlantAngle, true);
	memory_clean::safe_delete(_antenna.RxAntSpacing, true);
	memory_clean::safe_delete(__imta);
}

gtt_highspeed_config* gtt_highspeed::get_precise_config() {
	return (gtt_highspeed_config*)get_config();
}
