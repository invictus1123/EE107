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

	def set_headings(self, current_x, current_y):
		if (self.last_x is None):
			target_norm = math.hypot(self.target_x, self.target_y);
			self.target_heading_x = self.target_x/target_norm;
			self.target_heading_y = self.target_y/target_norm;
			self.current_heading_x = 0;
			self.current_heading_y = 1;
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
		return math.atan2(self.target_heading_x, self.target_heading_y) \
		- math.atan2(self.current_heading_x, self.current_heading_y);

