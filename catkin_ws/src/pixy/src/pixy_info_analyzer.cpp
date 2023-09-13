#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Vector3.h>


/*-------------------*/
const int stampSpeed = 100;
std::vector<int> okColorsId = { 0 };
std::vector<std::string> okCodes = { "Yo" };
/*-------------------*/
ros::Publisher pubLcd;
ros::Publisher pubMotors;
ros::Publisher pubPixyFinished;

void SendLcd(std::string msg)
{
    std_msgs::String lcdMsg;
    lcdMsg.data = msg;
    pubLcd.publish(lcdMsg);
}


void sendMotorMessage(double x, double y) {
    geometry_msgs::Vector3 robot_msg;
    robot_msg.x = x;
    robot_msg.y = y;
    pubMotors.publish(robot_msg);
}


void PixyFinish()
{
    SendLcd("-Pixy a termine -");

    std_msgs::Bool msg;
    msg.data = true;
    pubPixyFinished.publish(msg);
}

void StampOn() {
    SendLcd("-  Tamponnage  -");

    // Stamp On
    sendMotorMessage(stampSpeed, 6);
    ros::Duration(2.0).sleep();

    // Stamp Off
    sendMotorMessage(stampSpeed, 7);
    ros::Duration(2.0).sleep();

    // Stamp Stop
    sendMotorMessage(stampSpeed, 8);
    PixyFinish();
}


void StampedCallback(const std_msgs::Bool& msg)
{
    //PixyFinish();
}

void ReceiveCode(const std_msgs::String& msg)
{
    bool doStamp = false;

    for(int i = 0; i < okCodes.size(); i++)
    {
        if(msg.data == okCodes.at(i))
        {
            doStamp = true;
            break;
        }
    }

    if(doStamp)
        StampOn();
    else
        PixyFinish();
}

void ReceiveColor(const std_msgs::Float32& msg)
{
    bool doStamp = false;

    int colorId = (int)msg.data;
    for(int i = 0; i < okColorsId.size(); i++)
    {
        if(colorId == okColorsId.at(i))
        {
            doStamp = true;
            break;
        }
    }

    if(doStamp)
        StampOn();
    else
        PixyFinish();
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "pixy_node_analyser");
    ros::NodeHandle nh;

    //Subs
    ros::Subscriber receiveCode = nh.subscribe("/pixy_stringCode", 10, ReceiveCode);
    ros::Subscriber receiveColor = nh.subscribe("/pixy_colorCode", 10, ReceiveColor);
    ros::Subscriber stampCallback = nh.subscribe("/stampCallback", 10, StampedCallback);

    //Pubs
    pubLcd = nh.advertise<std_msgs::String>("/setLcdText", 10);
    pubPixyFinished = nh.advertise<std_msgs::Bool>("/pixy_finished", 10);
    pubMotors = nh.advertise<geometry_msgs::Vector3>("/motorsControl", 10);
    ros::Duration(2.0).sleep();

    

    ros::spin();
    return 0;
}