import sys
import struct
import traceback
import time
import socket
import math

import roomba_class
import roomba_move
from constants import *

portno = 23000; # 6000 if getting real data, 23000 for emulator data
laptopHostname = socket.gethostbyname(socket.gethostname())
serverHostname = 'ee107server.stanford.edu';
macNUC = 'C03FD560BEEF'; # 12 characters
portToReadData = 0;

# Connect to server, get port number for localizer data
try:
    sock = socket.create_connection((serverHostname, portno));
    sock.sendall(macNUC);
    rcvStr = sock.recv(SIZEOF_INT);
    portToReadData = struct.unpack("@i", rcvStr)[0];
    if (portToReadData != -1):
    	print "Localizer asked to listen at Port %d." % portToReadData;
    else:
    	print "Localizer says the MAC address are already on the list."
    	print "Exiting..."
    	exit(0);
    sock.close();
except Exception, e:
	traceback.print_exc();
	sock.close();
	exit(0);

# Read localizer data
try:
	(connSock, connAddr) = (None, None);
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1); # Allow address reuse
	sock.bind((laptopHostname, portToReadData));
	sock.listen(5);
except Exception, e:
	traceback.print_exc();
 	sock.close();
 	exit(0);

newData = "";

# Intialize waypoints and Roomba objects
waypoints = [[100,0], [50,0]];
current_waypoint = 0;
roomba_pos = roomba_class.Roomba_Position(waypoints[0][0], waypoints[0][1]); #initialize with the first waypoints as the target

while (current_waypoint != DONE_FLAG):
	try:
		print "Listening at port %d..." % portToReadData;
		(connSock, connAddr) = sock.accept();
		rcvStr = connSock.recv(lengthOfData*SIZEOF_FLOAT);
		newData = struct.unpack("@" + str(lengthOfData) + "f", rcvStr);

		x_estimate, y_estimate = newData[2], newData[3]; # TODO
		(q0, q1, q2, q3) = (newData[8],newData[9],newData[10],newData[11]);

		print "Data received from port %d" % portToReadData;
		print "Timestamp: %f s, %f ms" % (newData[0], newData[1]);
		print "Position (%f, %f, %f)" % (newData[2],newData[3],newData[4]);
		print "Accelerometer (%f, %f, %f)" % (newData[5],newData[6],newData[7]);
		print "Orientation quat (%f, %f, %f, %f)" % (newData[8],newData[9],newData[10],newData[11]);
		print "Yaw from quat: %f" % math.atan2(2.0*(q0*q3 + q1*q2), 1.0 - 2.0*(q2*q2 + q3*q3));

		current_waypoint = roomba_move.move_loop(roomba_pos, waypoints, current_waypoint,
			x_estimate, y_estimate, q0, q1, q2, q3);

		connSock.close();
		time.sleep(.01);
	except Exception, e:
		traceback.print_exc();
	 	sock.close();
	 	exit(0);

sock.close();

