/*
* =====================================================================================
*
*       Filename:  vue.cpp
*
*    Description:  车辆类实现
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
#include"function.h"

using namespace std;

vue::vue() {
	m_physics_level = new vue_physics();

	m_physics_level->set_superior_level(this);
}

vue::~vue() {
	memory_clean::safe_delete(m_physics_level);
}

vue_physics* vue::get_physics_level() {
	return m_physics_level;
}
