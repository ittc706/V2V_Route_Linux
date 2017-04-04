/*
* =====================================================================================
*
*       Filename:  config.cpp
*
*    Description:  配置文件类
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

#include<iostream>
#include<fstream>
#include<sstream>
#include"config.h"
#include"context.h"
#include"config_loader.h"
#include"function.h"

using namespace std;

void global_control_config::load() {
	//首先先判断当前的平台，利用路径的表示在两个平台下的差异来判断
	ifstream inPlatformWindows("config\\global_control_config.xml"),
		inPlatformLinux("config/global_control_config.xml");

	if (inPlatformWindows.is_open()) {
		set_platform(Windows);
		cout << "您当前的平台为：Windows" << endl;
	}
	else if (inPlatformLinux.is_open()) {
		set_platform(Linux);
		cout << "您当前的平台为：Linux" << endl;
	}
	else
		throw logic_error("PlatformError");


	//开始解析配置文件
	switch (get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\global_control_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/global_control_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;

	if ((temp = get_config_loader()->get_param("ntti")) != nullString) {
		set_ntti(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("gtt_mode")) != nullString) {
		if (temp == "HIGHSPEED")
			set_gtt_mode(HIGHSPEED);
		else if (temp == "URBAN")
			set_gtt_mode(URBAN);
		else
			throw logic_error("ConfigLoaderError");
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("route_mode")) != nullString) {
		if (temp == "TCP")
			set_route_mode(TCP);
		else
			throw logic_error("ConfigLoaderError");
	}
	else
		throw logic_error("ConfigLoaderError");

	cout << "ntti: " << get_ntti() << endl;
	cout << "gtt_mode: " << (get_gtt_mode() == URBAN ? "URBAN" : "HIGHSPEED") << endl;
	cout << "route_mode: " << (get_route_mode() == TCP ? "TCP" : "ERROR") << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

gtt_config* gtt_config::gtt_config_bind_by_mode(gtt_mode t_mode) {
	if (t_mode == HIGHSPEED) {
		return new gtt_highspeed_config();
	}
	else {
		return new gtt_urban_config();
	}
}

void gtt_highspeed_config::load() {

	//开始解析系统配置文件
	switch (context::get_context()->get_global_control_config()->get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\gtt_highspeed_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/gtt_highspeed_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;

	if ((temp = get_config_loader()->get_param("road_length")) != nullString) {
		set_road_length(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");


	if ((temp = get_config_loader()->get_param("road_width")) != nullString) {
		set_road_width(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("speed")) != nullString) {
		set_speed(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("freshtime")) != nullString) {
		set_freshtime(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	cout << "road_length: " << get_road_length() << endl;
	cout << "road_width: " << get_road_width() << endl;
	cout << "speed: " << get_speed() << endl;
	cout << "freshtime: " << get_freshtime() << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

void gtt_urban_config::load() {
	//开始解析系统配置文件
	switch (context::get_context()->get_global_control_config()->get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\gtt_urban_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/gtt_urban_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;

	if ((temp = get_config_loader()->get_param("road_length_ew")) != nullString) {
		set_road_length_ew(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("road_length_sn")) != nullString) {
		set_road_length_sn(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");


	if ((temp = get_config_loader()->get_param("road_width")) != nullString) {
		set_road_width(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("speed")) != nullString) {
		set_speed(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("freshtime")) != nullString) {
		set_freshtime(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	cout << "road_length_ew: " << get_road_length_ew() << endl;
	cout << "road_length_sn: " << get_road_length_sn() << endl;
	cout << "road_width: " << get_road_width() << endl;
	cout << "speed: " << get_speed() << endl;
	cout << "freshtime: " << get_freshtime() << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

void rrm_config::load() {
	//开始解析系统配置文件
	switch (context::get_context()->get_global_control_config()->get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\rrm_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/rrm_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;

	if ((temp = get_config_loader()->get_param("total_bandwidth")) != nullString) {
		int t_total_bandwidth = stoi(temp);
		t_total_bandwidth *= 1000 * 1000;
		set_total_bandwidth(t_total_bandwidth);
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("rb_num_per_pattern")) != nullString) {
		set_rb_num_per_pattern(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("drop_sinr_boundary")) != nullString) {
		set_drop_sinr_boundary(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("select_altorithm")) != nullString) {
		set_select_altorithm(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("time_division_granularity")) != nullString) {
		set_time_division_granularity(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	//通过获取的配置信息，计算pattern数量
	set_pattern_num();

	cout << "total_bandwidth: " << get_total_bandwidth() << endl;
	cout << "rb_num_per_pattern: " << get_rb_num_per_pattern() << endl;
	cout << "pattern_num: " << get_pattern_num() << endl;
	cout << "drop_sinr_boundary: " << get_drop_sinr_boundary() << endl;
	cout << "select_altorithm: " << get_select_altorithm() << endl;
	cout << "time_division_granularity: " << get_time_division_granularity() << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

void tmc_config::load() {
	//开始解析系统配置文件
	switch (context::get_context()->get_global_control_config()->get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\tmc_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/tmc_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;

	if ((temp = get_config_loader()->get_param("package_num")) != nullString) {
		set_package_num(stoi(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	if ((temp = get_config_loader()->get_param("trigger_rate")) != nullString) {
		set_trigger_rate(stod(temp));
	}
	else
		throw logic_error("ConfigLoaderError");

	cout << "package_num: " << get_package_num() << endl;
	cout << "trigger_rate: " << get_trigger_rate() << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}


void route_config::load() {
	//开始解析系统配置文件
	switch (context::get_context()->get_global_control_config()->get_platform()) {
	case Windows:
		get_config_loader()->resolv_config_file("config\\route_config.xml");
		break;
	case Linux:
		get_config_loader()->resolv_config_file("config/route_config.xml");
		break;
	default:
		throw logic_error("Platform Config Error!");
	}

	const string nullString("");
	string temp;


	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}