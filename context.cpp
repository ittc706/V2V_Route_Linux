/*
* =====================================================================================
*
*       Filename:  容器类.cpp
*
*    Description:  系统类实现
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

#include"context.h"
#include"function.h"
#include"system_control.h"
#include"config_loader.h"
#include"config.h"
#include"route.h"
#include"gtt_highspeed.h"
#include"gtt_urban.h"
#include"tmc.h"
#include"wt.h"
#include"vue.h"

using namespace std;

context* context::s_singleton_context = nullptr;

context* context::get_context() {
	if (s_singleton_context == nullptr) {
		context* __context = new context();
		__context->singleton_initialize();
		__context->dependcy_inject();
		s_singleton_context = __context;
		__context->post_processor();
	}
	return s_singleton_context;
}

context::context() {

}

void context::singleton_initialize() {
	set_system_control(new system_control());
	set_config_loader(new config_loader());
	set_global_control_config(new global_control_config());
	set_gtt_config(gtt_config::gtt_config_bind_by_mode(get_global_control_config()->get_gtt_mode()));
	set_rrm_config(new rrm_config());
	set_tmc_config(new tmc_config());
	set_route(route::route_bind_by_mode(get_global_control_config()->get_route_mode()));
	set_gtt(gtt::gtt_bind_by_mode(get_global_control_config()->get_gtt_mode()));
	set_tmc(new tmc());
	set_wt(new wt());
}

void context::dependcy_inject() {
	get_system_control()->set_context(this);
	get_global_control_config()->set_config_loader(get_config_loader());
	get_gtt_config()->set_config_loader(get_config_loader());
	get_rrm_config()->set_config_loader(get_config_loader());
	get_tmc_config()->set_config_loader(get_config_loader());
	get_gtt()->set_config(get_gtt_config());
}

void context::post_processor() {
	get_global_control_config()->load();
	get_gtt_config()->load();
	get_rrm_config()->load();
	get_tmc_config()->load();
	get_gtt()->initialize();
	get_route()->initialize();
	wt::set_resource();
}

context::~context() {
	memory_clean::safe_delete(m_system_control);
	memory_clean::safe_delete(m_config_loader);
	memory_clean::safe_delete(m_global_control_config);
	memory_clean::safe_delete(m_gtt_config);
	memory_clean::safe_delete(m_rrm_config);
	memory_clean::safe_delete(m_tmc_config);
	memory_clean::safe_delete(m_gtt);
	memory_clean::safe_delete(m_tmc);
	memory_clean::safe_delete(m_wt);
	memory_clean::safe_delete(m_vue_array, true);
}