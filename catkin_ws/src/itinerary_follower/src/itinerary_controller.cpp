#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Bool.h>
#include "../include/AllPoints.h"
#include <cmath>



AllPoints allPoints;
int actualItinerary = 0;
int actualPoint = 0;
Point destination;
ros::Publisher pubMotors;
int motorsSpeed = 125;
int turningSpeed = 175;
enum class State {None, Turning, MovingX, MovingY};
State robotState;
double tolerance = 0.05;

double robot_x;
double robot_y; 

void TurnRobot(double angle)
{
    geometry_msgs::Vector3 robot_msg;
    robot_msg.x = static_cast<float>(angle);
    robot_msg.y = 5;
    robot_msg.z = turningSpeed;
    pubMotors.publish(robot_msg);
}

void Move()
{
    double diffX = std::abs(destination.x) - std::abs(robot_x);
    double diffY = std::abs(destination.y) - std::abs(robot_y);
    robotState = diffX > diffY ? State::MovingX : State::MovingY;

    geometry_msgs::Vector3 robot_msg;
    robot_msg.y = 1;
    robot_msg.x = motorsSpeed;
    pubMotors.publish(robot_msg);
}

void StartPoint()
{
    destination = allPoints.getPoint(actualItinerary, actualPoint);
    if(actualItinerary != 0 && actualPoint != 0)
    {
        robotState = State::Turning;
        TurnRobot(destination.z);
    } else {
        Move();
    }
}


void StartMove(const std_msgs::Bool& msg)
{
    Move();
}
void StartMove()
{
    Move();
}
void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    robot_x = msg->pose.position.x;
    robot_y = msg->pose.position.y;
    ROS_INFO("x: %f, y: %f", robot_x, robot_y);

    if(robotState == State::MovingX) 
    {
        if(robot_x == destination.x)
        {
            robotState = State::None;
             ros::Duration(1.0).sleep();
        }
    }
    else if(robotState == State::MovingY)
    {
        if(robot_y == destination.y)
        {
            robotState = State::None;
             ros::Duration(1.0).sleep();
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "itineraryController");
    ros::NodeHandle nh;

    ros::Subscriber pose_sub = nh.subscribe("/slam_out_pose", 1000, poseCallback);
    pubMotors = nh.advertise<geometry_msgs::Vector3>("/motorsControl", 10);

    ros::spin();
    return 0;
}
