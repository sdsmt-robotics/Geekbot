#!/usr/bin/env bash
PATH=/bin:/usr/bin:/usr/local/bin
export PATH
WS_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && cd ../../.. && pwd )"

source /opt/ros/kinetic/setup.bash
source $WS_PATH/devel/setup.bash

LOCAL_IP=$(ip route get 8.8.8.8 | awk 'NR==1 {print $NF}') # Grab the device's internet-connected IP
LOCAL_PORT=11311



export ROS_MASTER_URI=http://$LOCAL_IP:$LOCAL_PORT
export ROS_IP=$LOCAL_IP



roslaunch bringup_pkg geekbot.launch
