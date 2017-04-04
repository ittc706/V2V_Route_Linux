#pragma once

#include"gtt.h"

class gtt_highspeed_config;

class gtt_highspeed:public gtt{
	/*--------------------接口--------------------*/
public:
	void initialize() override;

	int get_vue_num() override;

	void fresh_location() override;

	void calculate_pl(int t_vue_id1, int t_vue_id2) override;

	gtt_highspeed_config* get_precise_config();
};