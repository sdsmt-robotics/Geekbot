#!/usr/bin/env python

import geekbot
import time

lights = False

geek = geekbot.Robot("/dev/ttyUSB0", 57600)
geek.lights_on()
time.sleep(.5)
geek.lights_off()


#geek.set_ir_position(-30000)
#time.sleep(2)
#print(geek.get_ir_distance())
#geek.set_ir_position(30000)
#time.sleep(2)
#print(geek.get_ir_distance())
geek.set_ir_position(0)
#time.sleep(2)

#geek.drive_forward(12000, 2)

while 1:
	geek.drive_forward(10000)
	dist = 50
	while dist  > 30:
		dist = geek.get_ir_distance()
		print(dist)
		time.sleep(0.1)
	geek.drive_left_wheel(5000)
	geek.drive_right_wheel(-5000)
	time.sleep(.5)
	geek.halt()
	time.sleep(.25)

geek.drive_left_wheel(30000)
time.sleep(2)
geek.drive_left_wheel(0)

geek.drive_right_wheel(30000)
time.sleep(2)
geek.drive_right_wheel(0)

