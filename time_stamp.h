#pragma once
#include<string>
#include"reflect/object.h"

class v2x_time:public object {
	REGISTE_MEMBER_HEAD(v2x_time)

private:
	int m_tti;
	void set_tti(std::string t_tti) {
		m_tti = stoi(t_tti);
	}
public:
	int get_tti() {
		return m_tti;
	}

	/*--------------------接口--------------------*/
public:
	void increasement() {
		++m_tti;
	}
};