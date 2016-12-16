import math

class Roomba_Position(object):
	def __init__(self, target_x, target_y):
		self.target_x = target_x;
		self.target_y = target_y;
		self.origin_x = None;
		self.origin_y = None;
		self.relative_x = None;
		self.relative_y = None;
		self.target_heading_x = None;
		self.target_heading_y = None;
		self.current_heading_x = None;
		self.current_heading_y = None;
		self.init_yaw = None;
		
	def get_movement_instructions(self, x_estimate,y_estimate, q0, q1, q2, q3):
		self.set_headings_quat(x_estimate,y_estimate, q0, q1, q2, q3);
		angle = self.get_angle();
		return angle;
		# print "Move straight at the following heading:";
		# my_heading_angle = math.atan2(self.current_heading_y,self.current_heading_x);
		# heading_angle_after_turn = my_heading_angle + angle;
		# effective_heading_x = math.cos(heading_angle_after_turn);
		# effective_heading_y = math.sin(heading_angle_after_turn);
		# print("x: ", effective_heading_x," y: ", effective_heading_y);

	# def set_headings(self, current_x, current_y):
	# 	if (self.last_x is None):
	# 		target_norm = math.hypot(self.target_x, self.target_y);
	# 		self.target_heading_x = self.target_x/target_norm;
	# 		self.target_heading_y = self.target_y/target_norm;
	# 		self.current_heading_x = 0;
	# 		self.current_heading_y = 1;
			
	# 		#initialize the last_x and last_y poistions
	# 		self.last_x = current_x;
	# 		self.last_y = current_y;
	# 	else:
	# 		self.target_heading_x = self.target_x - current_x;
	# 		self.target_heading_y = self.target_y - current_y;
	# 		target_norm = math.hypot(self.target_heading_x, self.target_heading_y);
	# 		self.target_heading_x /= target_norm;
	# 		self.target_heading_y /= target_norm;

	# 		self.current_heading_x = current_x - self.last_x;
	# 		self.current_heading_y = current_y - self.last_y;
	# 		current_norm = math.hypot(self.current_heading_x, self.current_heading_y);
	# 		self.current_heading_x /= current_norm;
	# 		self.current_heading_y /= current_norm;

	# 		self.last_x = current_x;
	# 		self.last_y = current_y;

	# Set heading using quaternion values from IMU
	def set_headings_quat(self, current_x, current_y, q0, q1, q2, q3):
		# First iterations setup
		if (self.init_yaw == None or self.origin_x == None):
			self.init_yaw = math.atan2(2.0*(q0*q3 + q1*q2), 1.0 - 2.0*(q2*q2 + q3*q3));
			self.origin_x = current_x;
			self.origin_y = current_y;

		self.relative_x = current_x - self.origin_x;
		self.relative_y = current_y - self.origin_y;
		self.target_heading_x = self.target_x - self.relative_x;
		self.target_heading_y = self.target_y - self.relative_y;
		target_norm = math.hypot(self.target_heading_x, self.target_heading_y);
		if (target_norm != 0):
			self.target_heading_x /= target_norm;
			self.target_heading_y /= target_norm;

		abs_yaw = math.atan2(2.0*(q0*q3 + q1*q2), 1.0 - 2.0*(q2*q2 + q3*q3));
		rel_yaw = abs_yaw - self.init_yaw;
		self.current_heading_x = -math.sin(rel_yaw);
		self.current_heading_y = math.cos(rel_yaw);

		print "CHeading: x: %f y: %f" % (self.current_heading_x, self.current_heading_y);
		print "THeading: x: %f y: %f" % (self.target_heading_x, self.target_heading_y);
		print "Current (relative) pos: x: %f y: %f" % (self.relative_x, self.relative_y);
		print "Target (relative) pos: x: %f y: %f" % (self.target_x, self.target_y);			
			
	def set_target(self, waypoint_x, waypoint_y):
		self.target_x = waypoint_x;
		self.target_y = waypoint_y;
		
	def get_angle(self):
		temp_angle = math.atan2(self.target_heading_y, self.target_heading_x) \
		- math.atan2(self.current_heading_y, self.current_heading_x);
		if (temp_angle > math.pi):
			return -(2*math.pi - temp_angle);
		elif(temp_angle < -math.pi):
			return (2*math.pi + temp_angle);
		else:
			return temp_angle;