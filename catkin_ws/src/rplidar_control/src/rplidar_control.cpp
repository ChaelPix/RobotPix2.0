#include <ros/ros.h>
#include <std_srvs/Empty.h>
#include <std_msgs/Int32.h>

ros::ServiceClient start_motor_client;
ros::ServiceClient stop_motor_client;
std_srvs::Empty srv;

void StartLidar()
{
    if (start_motor_client.call(srv))
        ROS_INFO("RPLIDAR motor started.");
    else
        ROS_ERROR("Failed to start RPLIDAR motor.");
}

void StopLidar()
{
    if (stop_motor_client.call(srv))
        ROS_INFO("RPLIDAR motor stopped.");
    else
        ROS_ERROR("Failed to stop RPLIDAR motor.");
}

void LidarControl(const std_msgs::Int32 &msg)
{
    msg.data == 1 ? StartLidar() : StopLidar();
}

int main(int argc, char **argv)
{
    //Node init
    ros::init(argc, argv, "rplidar_control");
    ros::NodeHandle nh;

    //Services
    start_motor_client = nh.serviceClient<std_srvs::Empty>("/start_motor");
    stop_motor_client = nh.serviceClient<std_srvs::Empty>("/stop_motor");

    //Subs
    ros::Subscriber lidarControl_sub = nh.subscribe("/lidarControl", 10, LidarControl);

    //----
    ros::spin();
    return 0;
}
