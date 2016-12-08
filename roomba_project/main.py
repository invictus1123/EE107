import create2api
import roomba_class

FEEDBACK_INTERVAL = .100 #seconds
bot = create2api.Create2()
#bot.start()
#bot.safe()

emulator_current_x = 0;
emulator_current_y = 0;
emulator_heading_x = 0;
emulator_heading_y = 1;

def emulator_drive_straight(velocity): #v in mm/s
	global emulator_current_x;
	global emulator_current_y;
	global emulator_heading_x;
	global emulator_heading_y;
	global FEEDBACK_INTERVAL;
	emulator_current_x += emulator_heading_x*(velocity/1000)*FEEDBACK_INTERVAL;
	emulator_current_y += emulator_heading_y*(velocity/1000)*FEEDBACK_INTERVAL;
	
def emulator_turn_Degrees(degrees):
	global emulator_heading_x;
	global emulator_heading_y;
	angle = atan2(emulator_heading_y , emulator_heading_x);
	angle+=degrees;
	emulator_heading_x = cos(angle);
	emulator_heading_y = sin(angle);
def getPositionEstimate()
	global emulator_current_x;
	global emulator_current_y;
	return(3.14159+emulator_current_x,1+emulator_current_y);

waypoints = [(10,10), (0,0) , (-5,0) , (5,0)];
for i in range(len(waypoints)):
	roomba_pos = Roomba_Position(waypoints[i][0],waypoints[i][1])
	print("Going to ( " + waypoints[i][0] + "," + waypoints[i][1] + ")")
if i==1:
#at the starting location, set whatever position estimate we get to the origin, (0,0).
	starting_x,starting_y = getPositionEstimate()
	roomba_pos.set_headings(0,0)
while ( (roomba_pos.current_x - waypoints[i][0] > .1) and (roomba_pos.current_y - waypoints[i][1] > .1) ):
	#get a position estimate
	(x,y) = getPositionEstimate()
	#subtract the starting offset to get our relative coordinates
	x -= starting_x;
	y -= starting_y; 
	#roomba_pos.set_headings(x,y);
	angle = roomba_pos.get_angle();
	#bot.turnDegrees(angle);
	emulator_turn_Degrees(angle);
	#bot.drive_straight(100);
 	emulator_drive_straight(100);
	time.sleep(FEEDBACK_INTERVAL);
	print("Current x: " + emulator_current_x + "\n");
	print("Current y: " + emulator_current_y + "\n");
	print("Current Heading: " + atan2(emulator_heading_y , emulator_heading_x) + "\n");
