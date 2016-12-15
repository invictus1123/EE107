#Function to be called in the while(1) loop of the socket script
import roomba_class
import math
import move_commands
from constants import *

def move_loop(roomba, roomba_pos, waypoints, current_waypoint, x_estimate, y_estimate, q0, q1, q2, q3):
	# Get angle to turn Roomba
	angle = roomba_pos.get_movement_instructions(x_estimate, y_estimate, q0, q1, q2, q3);

	# Check if waypoint has been reached
	if (math.fabs(roomba_pos.relative_x - waypoints[current_waypoint][0]) < THRESHOLD and 
		math.fabs(roomba_pos.relative_y - waypoints[current_waypoint][1]) < THRESHOLD):
		if (current_waypoint + 1 == len(waypoints)):
			print ("Final Destination Reached");

			# Stop Roomba, play a sound (maybe?)
			move_commands.roomba_stop(roomba);
			move_commands.roomba_sound(roomba);

			return DONE_FLAG;
		else:
			print ("Destination %d Reached" % (current_waypoint + 1));

			# Stop Roomba, play a sound (maybe?)
			move_commands.roomba_stop(roomba);

			roomba_pos.set_target(waypoints[current_waypoint + 1][0],waypoints[current_waypoint + 1][1]);
			return current_waypoint + 1;
	else:
		# If waypoint not reached, give movement commands to Roomba
		# if(math.fabs(angle)>ANGLE_THRESHOLD):
		# # Turn Roomba according to angle
		# 	#move_commands.roomba_turn(angle,roomba);
		# else:
		# # Move Roomba forward (doesn't stop until next move_loop iteration)
		# 	#move_commands.roomba_forward(roomba);
		# 	sleep(.01);
		return current_waypoint;
