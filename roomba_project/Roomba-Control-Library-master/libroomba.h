/*     
 *      Copyright 2011 Ben Matthews <ben@kc2vjw.com>
 *     
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 3 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
*/

#ifndef __LIBROOMBA__
#define __LIBROOMBA__
#include <stdint.h>
#define MAIN_BRUSH_MOTOR 0x04
#define VACUUM_MOTOR 0x02
#define SIDE_BRUSH_MOTOR 0x01

int roomba_init(char* device);

/* Put the roomba into safe mode */
/* Allows full control unless charger or cliff is detected */
int roomba_safe(int);

/* Allows full control w/o safety features */
int roomba_full(int);

/* Releases the port */
int roomba_free(int);

/* Powers off the Roomba */
int roomba_off(int);

/* Plays a predefined "song" */
/* Song number should be between 0 and 15 inclusive */
int roomba_play_song(int, char song);

/* Define a song
 * song: song number (0-15)
 * song_data[1-16][0] =  note (31-127 (others are assumed to be a rest))
 * song_data[1-16][1] = note duration in 1/64 seconds
 */
int roomba_define_song(int,char song, char song_data[][2], char length);

/* Set the state of the Roomba's cleaning motors
 * motor_status is set by or'ing the desired motor constants
 * (MAIN_BRUSH_MOTOR,VACUUM_MOTOR,SIDE_BRUSH_MOTOR)
 */
int roomba_set_motors(int,char motor_status);

/* Moves the Rooba's drive motors
 * speed is in mm/s (signed)
 * radius is turn radius in mm (signed, positive is right)
 * The below constants are special case radi
 */

#define STRAIGHT_RADIUS 0x8000
#define CLOCKWISE_INPLACE_RADIUS -1
#define CCLOCKWISE_INPLACE_RADIUS 1

/* delay per speed when ramping in uSeconds */
#define RAMP_DELAY 20
int16_t current_speed;

int roomba_drive(int,int16_t speed, int16_t radius);
int roomba_ramp(int roomba_fd, int16_t speed, int16_t radius,
		int16_t increment);

/* If the roomba is in a cleaning mode, tells it to dock if
 * it finds a base
 */
int roomba_force_seeking_dock(int);

/* Sensor Data */

struct roomba_sensor_data
{
	uint8_t bumps_wheeldrops;
	uint8_t wall;
	uint8_t cliff_left;
	uint8_t cliff_front_left;
	uint8_t cliff_front_right;
	uint8_t cliff_right;
	uint8_t virtual_wall;
	uint8_t motor_overcurrent;
	uint8_t dirt_detector_left;
	uint8_t dirt_detector_right;
	uint8_t remote_opcode;
	uint8_t buttons;
	int16_t distance; //mm
	int16_t angle; //mm
	uint8_t charging_state; //1-5:: see enum
	uint16_t voltage; //mV
	int16_t current; //mA
	int8_t temperature; //degC
	uint16_t charge; //mAh
	uint16_t capacity; //mAh
};
enum charge_state { not_charging=0, charging_recovery=1, charging=2,
	trickle_charging=3, waiting=4, charging_error=5};
#define MAX_BUTTON 1
#define CLEAN_BUTTON 2
#define SPOT_BUTTON 4
#define POWER_BUTTON 8

#define SIDE_BRUSH_OVERCURRENT 1
#define VACUUM_OVERCURRENT 2
#define MAIN_BRUSH_OVERCURRENT 4
#define DRIVE_RIGHT_OVERCURRENT 8
#define DRIVE_LEFT_OVERCURRENT 16

#define BUMP_RIGHT 1
#define BUMP_LEFT 2
#define DROP_RIGHT 4
#define DROP_LEFT 8
#define DROP_CASTER 16

#define ROOMBA_ANGLE_RAD(x) ((2*x)/258)
#define ROOMBA_ANGLE_DEG(x) (360*x)/(258 * M_PI)

int roomba_read_sensor_data(int,struct roomba_sensor_data*);

/* Set LEDs */
struct roomba_led_status
{
	uint8_t dirt_detect:1;
	uint8_t max:1;
	uint8_t clean:1;
	uint8_t spot:1;
	uint8_t status_red:1;
	uint8_t status_grn:1;
	uint8_t padding:2;
};
int roomba_set_leds(int,struct roomba_led_status*,uint8_t power_color,
		uint8_t power_intensity);
#endif
