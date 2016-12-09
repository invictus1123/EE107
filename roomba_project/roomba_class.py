import math

class Roomba_Position(object):
	def __init__(self, target_x, target_y):
		self.target_x = target_x;
		self.target_y = target_y;
		self.last_x = None;
		self.last_y = None;
		self.target_heading_x = None;
		self.target_heading_y = None;
		self.current_heading_x = None;
		self.current_heading_y = None;
		
	def get_movement_instructions(self, x_estimate,y_estimate):
		self.set_headings(x_estimate,y_estimate);
		angle = get_angle(self);
		print("Turn ", angle, " Degrees Clockwise");
		print("Move straight at the following heading:");
		my_heading_angle = math.atan2(self.heading_y,self.heading_x);
		heading_angle_after_turn = my_heading_angle + angle;
		effective_heading_x = cos(heading_angle_after_turn);
		effective_heading_y = sin(heading_angle_after_turn);
		print("x: ", effective_heading x," y: ", effective_heading_y);
		
	def set_headings(self, current_x, current_y):
		if (self.last_x is None):
			target_norm = math.hypot(self.target_x, self.target_y);
			self.target_heading_x = self.target_x/target_norm;
			self.target_heading_y = self.target_y/target_norm;
			self.current_heading_x = 0;
			self.current_heading_y = 1;
			
			#initialize the last_x and last_y poistions
			self.last_x = current_x;
			self.last_y = current_y;
		else:
			self.target_heading_x = self.target_x - current_x;
			self.target_heading_y = self.target_y - current_y;
			target_norm = math.hypot(self.target_heading_x, self.target_heading_y);
			self.target_heading_x /= target_norm;
			self.target_heading_y /= target_norm;

			self.current_heading_x = current_x - self.last_x;
			self.current_heading_y = current_y - self.last_y;
			current_norm = math.hypot(self.current_heading_x, self.current_heading_y);
			self.current_heading_x /= current_norm;
			self.current_heading_y /= current_norm;

			self.last_x = current_x;
			self.last_y = current_y;
			
	def set_target(self, waypoint_x, waypoint_y):
		self.target_x = waypoint_x;
		self.target_y = waypoint_y;
		
	def get_angle(self):
		return math.atan2(self.target_heading_y, self.target_heading_x) \
		- math.atan2(self.current_heading_y, self.current_heading_x);

