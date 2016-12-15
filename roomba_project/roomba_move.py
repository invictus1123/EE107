#Function to be called in the while(1) loop of the socket script
import roomba_class
import math
from constants import *

def move_loop(roomba, waypoints, current_waypoint, x_estimate, y_estimate, q0, q1, q2, q3):
	roomba.get_movement_instructions(x_estimate, y_estimate, q0, q1, q2, q3);
	print math.fabs(roomba.relative_x - waypoints[current_waypoint][0])
	print math.fabs(roomba.relative_y - waypoints[current_waypoint][1])
	if (math.fabs(roomba.relative_x - waypoints[current_waypoint][0]) < THRESHOLD and 
		math.fabs(roomba.relative_y - waypoints[current_waypoint][1]) < THRESHOLD):
		if (current_waypoint + 1 == len(waypoints)):
			print ("Final Destination Reached");
			return DONE_FLAG;
		else:
			print ("Destination %d Reached" % (current_waypoint + 1));
			roomba.set_target(waypoints[current_waypoint + 1][0],waypoints[current_waypoint + 1][1]);
			return current_waypoint + 1;
	else:
		return current_waypoint;