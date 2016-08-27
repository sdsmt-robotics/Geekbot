#!/usr/bin/env python
import rospy
import serial
from struct import pack
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy
from std_msgs.msg import Bool, Int16

lights_state = False
buzzer_state = False

def map_short_to_dec(short):
    if short == 0:
        return 0
    num = short / 32767.0
    return num

def map_short(num):                                                                      
    temp = num * 32767
    if temp > 32767:
        return 32767
    elif temp < -32767:
        return -32767 
    return int(num * 32767)

def joy_update(data):
    global lights_state, buzzer_state
    twist = Twist()
    twist.linear.x  = data.axes[1]
    twist.angular.z = data.axes[0]
    cmd_vel_pub.publish(twist)
    if lights_state != data.buttons[1]:
        lights_pub.publish(data.buttons[1])
        lights_state = data.buttons[1] 
    if buzzer_state != data.buttons[2]:
        buzzer_pub.publish(data.buttons[2])
        buzzer_state = data.buttons[2]
    #rate.sleep() 

def publisher():
    print("publisher()")
    global lights_pub, buzzer_pub, cmd_vel_pub, rate
    lights_pub  = rospy.Publisher("/geekbot/lights", Bool, queue_size=10)
    buzzer_pub  = rospy.Publisher("/geekbot/buzzer", Bool, queue_size=10)
    cmd_vel_pub = rospy.Publisher("/geekbot/cmd_vel", Twist, queue_size=10)
    print("Publishers assigned, trying to subscribe")
    rospy.init_node('geekbot_teleop', anonymous=False)
    rospy.Subscriber("joy", Joy, joy_update)
    #rate = rospy.Rate(10)
    rospy.spin()

if __name__ == '__main__':
    publisher()
    
