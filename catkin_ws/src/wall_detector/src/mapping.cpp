#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Vector3.h>

//--Variables
int turningSpeed = 175;
float distances[] = {0.25, 1, 0.5};
int angles[] = {90, 0};
int _actualStep = 0;
//----Pubs
ros::Publisher pubStartWallDetection;
ros::Publisher pubLcd;
ros::Publisher pubLed;
ros::Publisher pubMotors;


void SaveMapping()
{
    std_msgs::String lcdTxt;
    lcdTxt.data = "Mapping Finished";
    pubLcd.publish(lcdTxt);
    
    geometry_msgs::Vector3 ledAction;
    ledAction.y = 1;
    pubLed.publish(ledAction);

    std::string command = "rosrun map_server map_saver -f ~/robotPix/catkin_ws/maps/map";
    int result = system(command.c_str());
    if (result == 0)
    {
        ROS_INFO("Map saved successfully.");
    }
    else
    {
        ROS_ERROR("Failed to save map.");
    }
}

void StopMotors()
{
    geometry_msgs::Vector3 robot_msg;
    robot_msg.x = 0;
    robot_msg.y = 0;
    pubMotors.publish(robot_msg);
}

void PubWallDetection()
{
    std_msgs::Float32 dist;
    dist.data = distances[_actualStep];
    pubStartWallDetection.publish(dist);
}

void AngleReached(const std_msgs::Bool& msg)
{
    std_msgs::String lcdTxt;
    lcdTxt.data = "En avant !";
    ROS_INFO("Angle Ok!"); 
    pubLcd.publish(lcdTxt);

    ros::Duration(2.0).sleep();
    PubWallDetection();
}

void WallDetected(const std_msgs::Bool& msg)
{
    StopMotors();
    _actualStep++;

    if(_actualStep < 3)
    {
        std_msgs::String lcdTxt;
        lcdTxt.data = "MAP : Mur Devant";
        pubLcd.publish(lcdTxt);

        ros::Duration(2.0).sleep();

        ROS_INFO("Rotating!"); 
        geometry_msgs::Vector3 robot_msg;
        robot_msg.x = angles[_actualStep-1];
        robot_msg.y = 5;
        robot_msg.z = turningSpeed;
        pubMotors.publish(robot_msg);
    } else 
    {
        ROS_INFO("Saving Map!"); 
        SaveMapping();
    }
}

int main(int argc, char** argv) {
    //Node init
    ros::init(argc, argv, "wall_mapping");
    ros::NodeHandle nh;

    //Subs
    ros::Subscriber lidar_sub = nh.subscribe("/frontWallDetection", 10, WallDetected);
    ros::Subscriber angle_sub = nh.subscribe("/RotateCallBack", 10, AngleReached);

    //Pubs
    pubStartWallDetection = nh.advertise<std_msgs::Float32>("/startWallDetection", 10);
    pubMotors = nh.advertise<geometry_msgs::Vector3>("/motorsControl", 10);

    pubLcd = nh.advertise<std_msgs::String>("/setLcdText", 10);
    pubLed = nh.advertise<geometry_msgs::Vector3>("/setButtonState", 10);
    ros::Duration(2.0).sleep();
   
    //----
    PubWallDetection();

    ros::spin();
    return 0;
}