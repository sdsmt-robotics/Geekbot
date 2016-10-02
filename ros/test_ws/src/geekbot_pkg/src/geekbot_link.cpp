#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/time.h>
#include <thread>

#include "easy_serial.h"
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Bool.h>

#define BAUDRATE 57600
#define TIMEOUT 50
#define READ_DELAY 10
#define IR_POLL 100

using std::string;
using std::cout;
using std::vector;
using std::endl;

struct motor_speeds
{
	short left;
	short right;
} motors;

volatile int ir_distance; 
volatile int main_thread_dead = 0;
volatile int main_thread;

int arduino = 0;

short map_short(float num)
{
    short temp = num * 32767;
    if (temp > 32767)
        return 32767;
    else if (temp < -32767)
        return -32767;

    return temp;
}

bool send_cmd(unsigned char flag, short data)
{
	unsigned char flag_temp = flag;
	short data_temp = data;
	int n  = write(arduino, &flag_temp, 1);
	n += write(arduino, &data_temp, sizeof(data_temp));
	if(n > 1 + sizeof(data));
		return 0;
	return 1;
}

bool set_left(short data){
    return send_cmd(0x36, data);
}

bool set_right(short data){
    return send_cmd(0x35, data);
}

bool lights_callback(std_msgs::Bool thing){
    if (thing.data){
        send_cmd(0x30, 1);
        cout << "Lights callback: on\n";
        }
    else{
        send_cmd(0x30, 0);
        cout << "Lights callback: off\n";
	}
}

bool buzzer_callback(std_msgs::Bool thing){
    if (thing.data){
        send_cmd(0x29, 1);
        cout << "Buzzer callback: on\n";
    }
    else{
        send_cmd(0x29, 0);
        cout << "Buzzer callback: off\n";
    }
}

bool ir_update(){
    //if(send_cmd(0x27, 1))
//	cout << "IR update error!" <<endl;
    /*unsigned char temp = 0x27;
    write(arduino, &temp, 1);
    write(arduino, &temp, 1);
    write(arduino, &temp, 1);*/
    if(send_cmd(0x27, 1))
        cout << "IR update send error." << endl;
    unsigned char high_byte = 0;
    unsigned char low_byte = 0;
    usleep(READ_DELAY*1000);
    read(arduino, &high_byte, 1);
    read(arduino, &low_byte, 1);
    short ir_temp = (high_byte << 8) | low_byte;
    if(ir_temp){
        ir_distance = ir_temp;
        return 0;
    }
    return 1;
}

void ir_func(int main_thread)
{
	while(!main_thread_dead){
	if(!ir_update())
	{
		cout << "ir_distance = " << ir_distance << endl;
	}
	usleep(IR_POLL * 1000);
	}
}

struct motor_speeds get_speeds(float linear, float angular)
{
	struct motor_speeds temp;
	temp.right = -linear - angular;
	temp.left  = -linear + angular;
	return temp;
}


bool cmd_vel_callback(geometry_msgs::Twist data){
    if (data.linear.x || data.angular.z){
        motors = get_speeds(data.linear.x, data.angular.z);
        set_left(motors.left);
        set_right(motors.right);
    }
    else if (data.linear.y || data.linear.z){
        set_left(data.linear.y);
        set_right(data.linear.z);
    }
}

int main(int argc, char **argv)
{
	arduino = serial_port_init("/dev/geekbot/arduino", BAUDRATE);
	//main_thread = std::this_thread::get_id();

	if(arduino < 1)
	{
		cout << "Can't open port, exiting...." << endl;
		main_thread_dead = 1;
		return 1;
	}
	//clear_port(arduino);
	unsigned char temp = 0;
	unsigned char handshake = 0x77;
	sleep(2);
	while(temp != handshake){	
        write(arduino, &handshake, 1);
		int n = read(arduino,&temp,1);
		usleep(10*1000);
		printf("Looking for handshake\n");
		if(n)printf("Handshake received: %0x\n", temp);}
    clear_port(arduino);
	std::thread ir_thread (ir_func, main_thread);
	sleep(10);
	main_thread_dead = 1;
	ir_thread.join();
	return 0;
}	
