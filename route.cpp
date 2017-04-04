#include"route.h"
#include"route_tcp.h"
using namespace std;

route* route::route_bind_by_mode(route_mode t_mode) {
	if (t_mode == TCP) {
		return new route_tcp();
	}
	else {
		throw logic_error("error");
	}
}