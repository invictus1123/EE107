#ifndef ROOMBA_FUNCS
#define ROOMBA_FUNCS

#include <math.h>

typedef struct roomba {
	float target_x = NULL;
	float target_y = NULL;
	float last_x = NULL;
	float last_y = NULL;
	float target_heading_x = NULL;
	float target_heading_y = NULL;
	float current_heading_x = NULL;
	float current_heading_y = NULL;
} Roomba;

void init_roomba_heading(Roomba r, float heading_x, float heading_y);

void set_roomba_target(Roomba r, float target_x, float target_y);

void set_headings(Roomba r, float current_x, float current_y);

float get_angle(Roomba r);

#endif /* ROOMBA_FUNCS */