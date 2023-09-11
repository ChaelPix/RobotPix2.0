#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Vector3.h>

ros::Publisher pubLcd;
ros::Publisher pubMotors;

std::vector<int> okColorsId = { 0 };
std::vector<std::string> okCodes = { "Bonjour" };

// void TamponOn()
// {
//     geometry_msgs::Vector3 robot_msg;
//     robot_msg.x = robotSpeed;
//     robot_msg.y = 5;
//     pubMotors.publish(robot_msg);
// }

// void TamponOff()
// {
//     geometry_msgs::Vector3 robot_msg;
//     robot_msg.x = robotSpeed;
//     robot_msg.y = 0;
//     pubMotors.publish(robot_msg);
// }


void ReceiveCode(const std_msgs::String& msg)
{
   // int colorId = (int)msg.data;

    for(int i = 0; i < okCodes.size(); i++)
    {
        
    }
}

void ReceiveColor(const std_msgs::Float32& msg)
{
    for(int i = 0; i < okCodes.size(); i++)
    {
        
    }
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "pixy_node_analyser");
    ros::NodeHandle nh;

    //Subs
    ros::Subscriber receiveCode = nh.subscribe("/pixy_stringCode", 10, ReceiveCode);
    ros::Subscriber receiveColor = nh.subscribe("/pixy_colorCode", 10, ReceiveColor);

    //Pubs
    pubLcd = nh.advertise<std_msgs::String>("/setLcdText", 10);

    ros::Duration(2.0).sleep();

    ros::spin();
    return 0;
}