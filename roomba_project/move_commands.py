from constants import *
import math
import time
def roomba_forward(bot):
	print "Roomba moving forward"
	bot.drive_straight(DRIVE_V);
	return 0;

def roomba_turn(angle,bot):
	print "Roomba turning %f radians clockwise" % angle;
	bot.turn_clockwise(TURN_V);
	time.sleep(6*angle/(2*math.pi))
	return 0;

def roomba_stop(bot):
	print "Roomba stopping"
	bot.drive(0,0);
	return 0;

def roomba_sound(bot):
	bot.play_test_sound();
	return 0;

def roomba_light(bot,RG_val):
	#turns the power button LED a particular color based on the RG value
	#0 = green, 255 = red, 128 = yellow
	#data to send to the bot: opcode = 139, code for lighting power LED = 4, RG value, Intensity = 255)
	data = (139,4,RG_val,255);
	bot.SCI.send(bot.config.data['opcodes']['led'],data);
	return 0;
