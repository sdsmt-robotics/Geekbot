#!/usr/bin/env python
import rospy
import serial
import threading
import time
from struct import pack
from collections import namedtuple
from geometry_msgs.msg import Twist
from std_msgs.msg import Bool, Int16

ser = serial.Serial('/dev/geekbot/arduino', 115200)
ser.timeout = None
time.sleep(.5)
print("Serial port opened.")
pub = rospy.Publisher('ir_mm', Int16, queue_size=5)

def map_short(num):
    temp = num * 32767
    if temp > 32767:
        return 32767
    elif temp < -32767:
        return -32767

    return int(num * 32767)
    
def pack_short(num):
    return pack("h", int(num))

def get_speeds(linear, angular):
    motors = namedtuple("motors",'left right')
    motors.right = -linear - angular
    motors.left  = -linear + angular
    return motors

def send_cmd(flag, data):
    ser.write(chr(flag))
    ser.write(pack_short(map_short(data)))

def left_motor(data):
    send_cmd(0x36, data)

def right_motor(data): 
    send_cmd(0x35, data)

def cmd_vel_callback(data):
    if data.linear.x or data.angular.z:
        speeds = get_speeds(data.linear.x, data.angular.z)
        left_motor(speeds.left)
        right_motor(speeds.right)
    elif data.linear.y or data.linear.z:
        left_motor(data.linear.y)
        right_motor(data.linear.z)

def lights_callback(thing):
    if thing.data == True:
        send_cmd(0x30, 1)
        print("Lights callback: on")
    elif thing.data == False:
        send_cmd(0x30, 0)
        print("Lights callback: off")

def buzzer_callback(thing):
    if thing.data == True:
        send_cmd(0x29, 1)
        print("Buzzer callback: on")
    elif thing.data == False:
        send_cmd(0x29, 0)
        print("Buzzer callback: off")

def ir_update(yes):
    if yes:
        send_cmd(0x27,1)
        mm = ser.read()*256
        #print(mm)
	mm = ser.read()+mm
	print(mm)
        pub.publish(mm) 
    else:
        yes = not yes
    return yes 
ir_update.counter = 0

if __name__ == '__main__':
    rospy.init_node('geekbot_link', anonymous=False)
    r = rospy.Rate(60)
    rospy.Subscriber("lights", Bool, lights_callback)
    rospy.Subscriber("cmd_vel", Twist, cmd_vel_callback)
    rospy.Subscriber("buzzer", Bool, buzzer_callback)
    update = False
    while not rospy.is_shutdown():
        r.sleep()
	update = ir_update(update)

    
