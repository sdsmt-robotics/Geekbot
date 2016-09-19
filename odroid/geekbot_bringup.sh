#!/bin/bash
LOCAL_IP=$(ip route get 8.8.8.8 | awk 'NR==1 {print $NF}') # Grab the device's internet-connected IP
LOCAL_PORT=11311

export ROS_MASTER_URI=http://$LOCAL_IP:$LOCAL_PORT
export ROS_IP=$LOCAL_IP

