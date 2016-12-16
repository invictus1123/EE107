#include "roomba_funcs.h"

void init_roomba_heading(Roomba r, float heading_x, float heading_y){
	r.current_heading_x = heading_x;
	r.current_heading_y = heading_y;
}

void set_roomba_target(Roomba r, float target_x, float target_y){
	r.target_x = target_x;
	r.target_y = target_y;
}

void set_headings(Roomba r, float current_x, float current_y){
	if (r.last_x == NULL){
		float target_norm = sqrt((r.target_x*r.target_x) + (r.target_y*r.target_y));
		r.target_heading_x = r.target_x/target_norm;
		r.target_heading_y = r.target_y/target_norm;
	} else {
		
	}
}

float get_angle(Roomba r);
