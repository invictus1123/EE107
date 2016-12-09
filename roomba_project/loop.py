#Function to be called in the while(1) loop of the socket script
import setup
def loop(Roomba_Position emu, x_estimate, y_estimate)
  if (emu.current_x - waypoints[current_waypoint][0] > .1 or emu.curren_y - waypoints[current_waypoint][1] >.1):
    emu.get_movement_instructions(x_estimate, y_estimate);
  else
    if current_waypoint + 1 = len(waypoints)
      print ("Final Destination Reached");
    else
      current_waypoint +=1;
      emu.set_target(waypoints[current_waypoint][0],waypoints[current_waypoint][1]);
      
