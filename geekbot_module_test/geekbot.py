import serial
from time import sleep as wait
from struct import pack, unpack
from collections import namedtuple


handshake  = 0x77
end_flag   = 0x33

drive_flag = 0x45
left_flag  = 0x36
right_flag = 0x35
lights_flag = 0x30

ir_read_flag = 0x27
ir_pos_flag  = 0x28

class Robot:

	def __init__(self, file, baud):
		self.location = file
		self.port = serial.Serial()
		self.port.baudrate = baud
		self.port.port = file
		self.port.timeout = 1
		self.port.dtr = 0
		self.port.open()
		wait(3)
		self.port.write(chr(handshake))
		wait(1)
		while self.port.read() != chr(0x77):
			print("Waiting for handshake")

	def shutdown(self):
		self.halt()
		self.port.close()

	def map_short(self, num): #where num is a num 0 - 100
    		temp = (num * 32767)/100
    		if temp > 32767:
    		    return 32767
    		elif temp < -32767:
    		    return -32767
   		return int(temp)

	def pack_short(self,num):
    		return pack("h", int(num))	

	def send_cmd(self,flag, data):
    		self.port.write(chr(flag))
    		self.port.write(self.pack_short(self.map_short(data)))

	def lights_on(self):
		self.send_cmd(lights_flag, 0x01)

	def lights_off(self):
		self.send_cmd(lights_flag, 0x00)

	def halt(self):
		self.send_cmd(drive_flag, 0)

	def turn(self, speed, seconds=None):
		self.send_cmd(left_flag, -speed)
		self.send_cmd(right_flag, speed)
		if seconds != None:
			wait(seconds)
			self.halt()
		return

	def drive_forward(self, speed, adjust=None, seconds=None):
		if adjust == None:
			self.send_cmd(drive_flag, speed)
		else: 
			self.send_cmd(left_flag, speed)
			adjusted = speed+adjust
			if   adjusted > 100:
				self.send_cmd(right_flag, 100)
			elif adjusted < 0:
				self.send_cmd(right_flag, 0)
			else:
				self.send_cmd(right_flag, speed+adjust)
		if seconds == None:
			return
		wait(seconds)
		self.halt()


	def drive_backward(self, speed, adjust=None, seconds=None):
		if adjust == None:
			self.send_cmd(drive_flag, -speed)
		else: 
			self.send_cmd(left_flag, -speed)
			adjusted = speed+adjust
			if   adjusted > 100:
				self.send_cmd(right_flag, -100)
			elif adjusted < 0:
				self.send_cmd(right_flag, -0)
			else:
				self.send_cmd(right_flag, -speed+adjust)
		if seconds == None:
			return
		wait(seconds)
		self.halt()

	def drive_right_wheel(self, speed):
		self.send_cmd(right_flag, -speed)
		
	def drive_left_wheel(self, speed):
		self.send_cmd(left_flag, -speed)

	def get_ir_distance(self):
		self.send_cmd(ir_read_flag, 1)
		data = self.port.read(2)
		dist = unpack(">H", data)
		return dist[0]

	def set_ir_position(self, angle):
		self.send_cmd(ir_pos_flag, angle)


