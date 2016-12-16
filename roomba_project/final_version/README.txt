To run:
	1) Setup hololens and IMU PCB on top of Roomba.
	2) Start hololens position tracking app and PCB serial data transmission.
	3) Connect the Roomba to your laptop via serial cable.
	4) Check the serial port of the Roomba serial connection (/dev/ttyUSB0 on windows/linux)
	5) Start the application by running: $ sudo python main_real.py [serial_port]
		Note: [serial_port] is the port found in step 4

To change waypoints:
	In main_real.py, change/add tuples to the 'waypoints' variable
