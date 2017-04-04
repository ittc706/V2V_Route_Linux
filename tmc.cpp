/*
* =====================================================================================
*
*       Filename:  tmc.cpp
*
*    Description:  业务模型与控制类实现
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

#include<random>
#include<fstream>
#include"context.h"
#include"gtt.h"
#include"tmc.h"
#include"vue.h"
#include"vue_physics.h"
#include"config.h"
#include"route_tcp.h"

using namespace std;

void tmc::statistic() {
	context* __context = context::get_context();
	ofstream success_event;
	if (context::get_context()->get_global_control_config()->get_platform() == Windows) {
		success_event.open("log\\success_event.txt");
	}
	else {
		success_event.open("log/success_event.txt");
	}

	route_tcp* __route = (route_tcp*)(__context->get_route());

	
	success_event << "total success event: " << __route->get_successful_event_vec().size() << endl;


	for (route_tcp_route_event* tcp_event : __route->get_successful_event_vec()) {
		success_event << tcp_event->to_string();
	}
	success_event << endl;
}