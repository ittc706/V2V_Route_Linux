#pragma once

#include"gtt.h"

class gtt_urban_config;

class gtt_urban :public gtt {
	/*--------------------接口--------------------*/
public:
	void initialize() override;

	int get_vue_num() override;

	void fresh_location() override;

	void calculate_pl(int t_vue_id1, int t_vue_id2) override;

	gtt_urban_config* get_precise_config();
}; 
