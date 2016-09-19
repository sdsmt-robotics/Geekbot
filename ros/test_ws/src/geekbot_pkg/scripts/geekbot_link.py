#!/usr/bin/env python
import rospy
import serial
from struct import pack
from collections import namedtuple
from geometry_msgs.msg import Twist
from std_msgs.msg import Bool, Int16

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
print("Serial port opened.")

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

def listener():
    rospy.init_node('geekbot_link', anonymous=False)
    rospy.Subscriber("lights", Bool, lights_callback)
    rospy.Subscriber("cmd_vel", Twist, cmd_vel_callback)
    rospy.Subscriber("buzzer", Bool, buzzer_callback)
    rospy.spin()

if __name__ == '__main__':
    listener()
    
