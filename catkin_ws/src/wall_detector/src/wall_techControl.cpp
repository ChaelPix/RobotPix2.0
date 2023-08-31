#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Vector3.h>

ros::Publisher pubStartWallDetection;
ros::Publisher pubLcd;
ros::Publisher pubLed;
ros::Publisher pubMotors;
bool _isInit = false;

void StopMotors()
{
    geometry_msgs::Vector3 robot_msg;
    robot_msg.x = 0;
    robot_msg.y = 0;
    pubMotors.publish(robot_msg);
}

void WallDetected(const std_msgs::Bool& msg)
{
    if(!_isInit)
        return;
    StopMotors();
    std_msgs::String lcdTxt;
    lcdTxt.data = "Mur Droit Devant";
    pubLcd.publish(lcdTxt);

    geometry_msgs::Vector3 ledAction;
    ledAction.y = 1;
    pubLed.publish(ledAction);
}

int main(int argc, char** argv) {
    //Node init
    ros::init(argc, argv, "wall_techControl");
    ros::NodeHandle nh;
    _isInit = true;
    //Subs
    ros::Subscriber lidar_sub = nh.subscribe("/frontWallDetection", 10, WallDetected);

    //Pubs
    pubStartWallDetection = nh.advertise<std_msgs::Float32>("/startWallDetection", 10);
    pubMotors = nh.advertise<geometry_msgs::Vector3>("/motorsControl", 10);

    pubLcd = nh.advertise<std_msgs::String>("/setLcdText", 10);
    pubLed = nh.advertise<geometry_msgs::Vector3>("/setButtonState", 10);
    ros::Duration(2.0).sleep();
   
    //----
    std_msgs::Float32 dist;
    dist.data = 0.2;
    pubStartWallDetection.publish(dist);

    ros::spin();
    return 0;
}
