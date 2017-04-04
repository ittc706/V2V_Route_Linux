 

#include"gtt.h"
#include"gtt_highspeed.h"
#include"gtt_urban.h"

using namespace std;

gtt* gtt::gtt_bind_by_mode(gtt_mode t_mode) {
	if (t_mode == HIGHSPEED) {
		return new gtt_highspeed();
	}
	else {
		return new gtt_urban();
	}
}