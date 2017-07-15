#!/usr/bin/env python

import geekbot
import time

geek = geekbot.Robot("/dev/ttyUSB0", 57600)
geek.lights_on()
time.sleep(.5)
geek.lights_off()

geek.set_ir_position(0)
time.sleep(.5)
geek.set_ir_position(-90)
time.sleep(.5)
geek.set_ir_position(90)
time.sleep(.5)
geek.set_ir_position(0)

geek.drive_left_wheel(-90)
time.sleep(1)
geek.drive_left_wheel(90)
time.sleep(1)
geek.drive_left_wheel(0)
geek.drive_right_wheel(-90)
time.sleep(1)
geek.drive_right_wheel(-90)
time.sleep(1)
geek.drive_right_wheel(0)

exit()

while 1:
	geek.drive_forward(60, -25)
	dist = 50
	while dist  > 30:
		dist = geek.get_ir_distance()
		print(dist)
		time.sleep(0.1)
	geek.drive_left_wheel(45)
	geek.drive_right_wheel(-45)
	time.sleep(.5)
	geek.halt()
	time.sleep(.25)

geek.drive_left_wheel(80)
time.sleep(2)
geek.drive_left_wheel(0)

geek.drive_right_wheel(80)
time.sleep(2)
geek.drive_right_wheel(0)

