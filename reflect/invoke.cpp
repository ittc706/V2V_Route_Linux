#include<stdexcept>
#include"invoke.h"
#include"../config.h"
#include"../system_control.h"
#include"../gtt_highspeed.h"
#include"../gtt_urban.h"
#include"../wt.h"
#include"../tmc.h"
#include"../route_tcp.h"
#include"../route_udp.h"
#include"../time_stamp.h"

REGISTE_CLASS_ID_RESOURCE(system_control)
REGISTE_CLASS_ID_RESOURCE(v2x_time)

REGISTE_CLASS_ID_RESOURCE(gtt_urban)
REGISTE_CLASS_ID_RESOURCE(gtt_highspeed)
REGISTE_CLASS_ID_RESOURCE(route_tcp)
REGISTE_CLASS_ID_RESOURCE(route_udp)
REGISTE_CLASS_ID_RESOURCE(wt);
REGISTE_CLASS_ID_RESOURCE(tmc);

REGISTE_CLASS_ID_RESOURCE(global_control_config)
REGISTE_CLASS_ID_RESOURCE(gtt_highspeed_config)
REGISTE_CLASS_ID_RESOURCE(gtt_urban_config)
REGISTE_CLASS_ID_RESOURCE(rrm_config)
REGISTE_CLASS_ID_RESOURCE(tmc_config)
REGISTE_CLASS_ID_RESOURCE(route_config)





/*
* 零元函数反射注册
*/
object* new_instance(const std::string& class_name) {
	FACTORY_INVOKE_START
		FACTORY_INVOKE(system_control)
		FACTORY_INVOKE(v2x_time)

		FACTORY_INVOKE(gtt_urban)
		FACTORY_INVOKE(gtt_highspeed)
		FACTORY_INVOKE(route_tcp)
		FACTORY_INVOKE(route_udp)
		FACTORY_INVOKE(tmc)
		FACTORY_INVOKE(wt)

		FACTORY_INVOKE(global_control_config)
		FACTORY_INVOKE(gtt_highspeed_config)
		FACTORY_INVOKE(gtt_urban_config)
		FACTORY_INVOKE(rrm_config)
		FACTORY_INVOKE(tmc_config)
		FACTORY_INVOKE(route_config)

		FACTORY_INVOKE_END
}

/*
* 零元函数反射注册
*/
void invoke(const object* obj, const std::string& method_name) {
	long class_id = obj->get_class_id();
	METHOD_INVOKE_CLASS_START(gtt_urban)
		METHOD_INVOKE_ZERO_PARAM(gtt_urban, initialize)
		METHOD_INVOKE_CLASS_END(gtt_urban)

		METHOD_INVOKE_CLASS_START(gtt_highspeed)
		METHOD_INVOKE_ZERO_PARAM(gtt_highspeed, initialize)
		METHOD_INVOKE_CLASS_END(gtt_highspeed)

		METHOD_INVOKE_CLASS_START(route_tcp)
		METHOD_INVOKE_ZERO_PARAM(route_tcp, initialize)
		METHOD_INVOKE_CLASS_END(route_tcp)

		METHOD_INVOKE_CLASS_START(route_udp)
		METHOD_INVOKE_ZERO_PARAM(route_udp, initialize)
		METHOD_INVOKE_CLASS_END(route_udp)

		METHOD_INVOKE_CLASS_START(wt)
		METHOD_INVOKE_ZERO_PARAM(wt, set_resource)
		METHOD_INVOKE_CLASS_END(wt)

		METHOD_INVOKE_CLASS_START(rrm_config)
		METHOD_INVOKE_ZERO_PARAM(rrm_config, set_pattern_num)
		METHOD_INVOKE_CLASS_END(rrm_config)
}

/*
* 函数反射注册
* 参数类型：int
*/
void invoke(const object* obj, const std::string& method_name, int param1) {
	long class_id = obj->get_class_id();

}

/*
* 函数反射注册
* 参数类型：long
*/
void invoke(const object* obj, const std::string& method_name, long param1) {
	long class_id = obj->get_class_id();


}

/*
* 函数反射注册
* 参数类型：float
*/
void invoke(const object* obj, const std::string& method_name, float param1) {
	long class_id = obj->get_class_id();


}

/*
* 函数反射注册
* 参数类型：double
*/
void invoke(const object* obj, const std::string& method_name, double param1) {
	long class_id = obj->get_class_id();

}

/*
* 函数反射注册
* 参数类型：void*
*/
void invoke(const object* obj, const std::string& method_name, void* param1) {
	long class_id = obj->get_class_id();

}

/*
* 函数反射注册
* 参数类型：void*
*/
void invoke(const object* obj, const std::string& method_name, const std::string& param1) {
	long class_id = obj->get_class_id();
	METHOD_INVOKE_CLASS_START(v2x_time)
		METHOD_INVOKE_ONE_PARAM(v2x_time, set_tti)
		METHOD_INVOKE_CLASS_END(v2x_time)

		METHOD_INVOKE_CLASS_START(global_control_config)
		METHOD_INVOKE_ONE_PARAM(global_control_config, set_ntti)
		METHOD_INVOKE_CLASS_END(global_control_config)


		METHOD_INVOKE_CLASS_START(gtt_highspeed_config)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed_config, set_road_length)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed_config, set_road_width)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed_config, set_speed)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed_config, set_freshtime)
		METHOD_INVOKE_CLASS_END(gtt_highspeed_config)

		METHOD_INVOKE_CLASS_START(gtt_urban_config)
		METHOD_INVOKE_ONE_PARAM(gtt_urban_config, set_road_length_ew)
		METHOD_INVOKE_ONE_PARAM(gtt_urban_config, set_road_length_sn)
		METHOD_INVOKE_ONE_PARAM(gtt_urban_config, set_road_width)
		METHOD_INVOKE_ONE_PARAM(gtt_urban_config, set_speed)
		METHOD_INVOKE_ONE_PARAM(gtt_urban_config, set_freshtime)
		METHOD_INVOKE_CLASS_END(gtt_urban_config)

		METHOD_INVOKE_CLASS_START(rrm_config)
		METHOD_INVOKE_ONE_PARAM(rrm_config, set_total_bandwidth)
		METHOD_INVOKE_ONE_PARAM(rrm_config, set_rb_num_per_pattern)
		METHOD_INVOKE_ONE_PARAM(rrm_config, set_drop_sinr_boundary)
		METHOD_INVOKE_ONE_PARAM(rrm_config, set_select_altorithm)
		METHOD_INVOKE_ONE_PARAM(rrm_config, set_time_division_granularity)
		METHOD_INVOKE_CLASS_END(rrm_config)

		METHOD_INVOKE_CLASS_START(tmc_config)
		METHOD_INVOKE_ONE_PARAM(tmc_config, set_package_num)
		METHOD_INVOKE_ONE_PARAM(tmc_config, set_hello_tti)
		METHOD_INVOKE_ONE_PARAM(tmc_config, set_trigger_rate)
		METHOD_INVOKE_CLASS_END(tmc_config)

		METHOD_INVOKE_CLASS_START(route_config)
		METHOD_INVOKE_ONE_PARAM(route_config, set_interval)
		METHOD_INVOKE_CLASS_END(route_config)
}


void invoke(const object* obj, const std::string& method_name, object* param1) {
	long class_id = obj->get_class_id();
	METHOD_INVOKE_CLASS_START(system_control)
		METHOD_INVOKE_ONE_PARAM(system_control, set_time)
		METHOD_INVOKE_ONE_PARAM(system_control, set_gtt)
		METHOD_INVOKE_ONE_PARAM(system_control, set_route)
		METHOD_INVOKE_ONE_PARAM(system_control, set_tmc)
		METHOD_INVOKE_ONE_PARAM(system_control, set_wt)
		METHOD_INVOKE_ONE_PARAM(system_control, set_global_control_config)
		METHOD_INVOKE_CLASS_END(system_control)


		METHOD_INVOKE_CLASS_START(gtt_urban)
		METHOD_INVOKE_ONE_PARAM(gtt_urban, set_config)
		METHOD_INVOKE_ONE_PARAM(gtt_urban, set_time)
		METHOD_INVOKE_ONE_PARAM(gtt_urban, set_rrm_config)
		METHOD_INVOKE_CLASS_END(gtt_urban)

		METHOD_INVOKE_CLASS_START(gtt_highspeed)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed, set_config)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed, set_time)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed, set_rrm_config)
		METHOD_INVOKE_CLASS_END(gtt_highspeed)

		METHOD_INVOKE_CLASS_START(route_tcp)
		METHOD_INVOKE_ONE_PARAM(route_tcp, set_gtt)
		METHOD_INVOKE_ONE_PARAM(route_tcp, set_wt)
		METHOD_INVOKE_ONE_PARAM(route_tcp, set_time)
		METHOD_INVOKE_ONE_PARAM(route_tcp, set_rrm_config)
		METHOD_INVOKE_ONE_PARAM(route_tcp, set_tmc_config)
		METHOD_INVOKE_ONE_PARAM(route_tcp, set_route_config)
		METHOD_INVOKE_CLASS_END(route_tcp)

		METHOD_INVOKE_CLASS_START(route_udp)
		METHOD_INVOKE_ONE_PARAM(route_udp, set_gtt)
		METHOD_INVOKE_ONE_PARAM(route_udp, set_wt)
		METHOD_INVOKE_ONE_PARAM(route_udp, set_time)
		METHOD_INVOKE_ONE_PARAM(route_udp, set_rrm_config)
		METHOD_INVOKE_ONE_PARAM(route_udp, set_tmc_config)
		METHOD_INVOKE_ONE_PARAM(route_udp, set_route_config)
		METHOD_INVOKE_CLASS_END(route_udp)
}
