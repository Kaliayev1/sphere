#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include <turtlesim/Pose.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "pid.h"

#define F 10

//Parametres PID globaux 
#define JoyMin -10
#define JoyMax 10

