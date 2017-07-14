import serial
import time
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
		time.sleep(3)
		self.port.write(chr(handshake))
		time.sleep(1)
		while self.port.read() != chr(0x77):
			print("Waiting for handshake")

	def map_short(self, num):
    		temp = num * 32767
    		if temp > 32767:
    		    return 32767
    		elif temp < -32767:
    		    return -32767
   		return int(num * 32767)

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

	def drive_forward(self, speed):
		self.send_cmd(drive_flag, speed)

	def drive_forward(self, speed, seconds=None):
		self.send_cmd(drive_flag, speed)
		if seconds == None:
			return
		time.sleep(seconds)
		self.halt()

	def drive_backward(self, speed):
		self.send_cmd(drive_flag, -speed)

	def drive_backward(self, speed, seconds=None):
		self.send_cmd(drive_flag, -speed)
		if seconds == none:
			return
		time.sleep(seconds)
		self.halt()

	def drive_right_wheel(self, speed):
		self.send_cmd(right_flag, speed)
		
	def drive_left_wheel(self, speed):
		self.send_cmd(left_flag, speed)

	def get_ir_distance(self):
		self.send_cmd(ir_read_flag, 1)
		data = self.port.read(2)
		dist = unpack(">H", data)
		return dist[0]

	def set_ir_position(self, angle):
		self.send_cmd(ir_pos_flag, angle)


