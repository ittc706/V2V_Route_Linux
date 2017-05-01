#pragma once
#include<string>
#include"micro.h"

/*
* 定义了反射的基类
*/
class object {
	INVOKE_FRIEND_DECLARED
public:
	void* operator new(size_t size, long class_id) {
		//对分配一个long，作为类型头
		void* raw = ::operator new(size + sizeof(long));
		char* pc = (char*) raw;
		*((long*)pc) = class_id;
		return pc + sizeof(long);
	}

	void operator delete(void *p) {
		::operator delete ((char*)p - sizeof(long));
	}

public:
	/*
	* 获取类型标记
	* 将其设置为虚函数,非常神奇,可以消除由于子类继承自带有虚函数的其他基类所造成的的内存偏移
	*/
	virtual long get_class_id() const {
		return *((long*)((char*)this - sizeof(long)));
	}

protected:
	static long class_id;
};


