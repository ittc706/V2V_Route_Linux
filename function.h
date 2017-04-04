#pragma once

#include<iostream>
#include<iomanip>
#include<vector>

class memory_clean {
public:
	/*
	* 删除函数，删除后会置空指针
	*/
	template<typename T>
	static void safe_delete(T*&t_point, bool t_is_array = false) {
		if (t_point != nullptr) {
			if (t_is_array)
				delete[] t_point;
			else
				delete t_point;
			t_point = nullptr;
		}
	}
};

class array_print {
public:
	template<typename T>
	static void print_vector_dim1(std::vector<T> t_vector) {
		for (T& t : t_vector)
			std::cout << std::left << std::setw(5) << t;
		std::cout << std::endl;
	}

	template<typename T>
	static void print_vector_dim2(std::vector<std::vector<T>> t_vector) {
		for (std::vector<T>& innerVector : t_vector) {
			for (T& t : innerVector) {
				std::cout << std::left << std::setw(5) << t;
			}
			std::cout << std::endl;
		}
	}
};