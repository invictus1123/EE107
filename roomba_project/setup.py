#Initializes a Roomba_Position object and the desired waypoints
import roomba_class
def setup()
  waypoints = [(50,50)];
  current_waypoint = 0
  emulator = Roomba_Position(waypoints[0][0], waypoints[0][1]); #initialize with the first waypoints as the target

