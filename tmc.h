#pragma once
#include"reflect/object.h"


class tmc :public object{
	REGISTE_MEMBER_HEAD(tmc)
public:
	/*
	* 统计仿真各项信息
	*/
	void statistic();
};