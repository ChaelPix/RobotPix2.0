#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Vector3.h>

const float robotSpeed = 150;
ros::Publisher pubMotors;
ros::Publisher pubWallFront;
bool isDetectingWall;

float wallDistance = 0.2; //~ 10cm

void PublishToMotors(int action)
{
    geometry_msgs::Vector3 robot_msg;
    robot_msg.x = robotSpeed;
    robot_msg.y = action;
    pubMotors.publish(robot_msg);
}

void lidarCallback(const sensor_msgs::LaserScan::ConstPtr& msg) {

    if (!isDetectingWall)
        return;

    const float WALL_DISTANCE_THRESHOLD = wallDistance;

    // Intervalles d'angles pour détecter un mur devant (en radians)
    const float MIN_ANGLE = -15 * M_PI / 180;  // -15 degrés convertis en radians
    const float MAX_ANGLE = 15 * M_PI / 180;   

    // Calculer les indices correspondants dans le tableau de mesures du LIDAR
    int min_index = (MIN_ANGLE - msg->angle_min) / msg->angle_increment;
    int max_index = (MAX_ANGLE - msg->angle_min) / msg->angle_increment;
  
    for (int i = min_index; i <= max_index; i++) {
        if (msg->ranges[i] < WALL_DISTANCE_THRESHOLD) {

            ROS_INFO("Wall in Front!"); 
            isDetectingWall = false;
            std_msgs::Bool msg;
            msg.data = true;
            pubWallFront.publish(msg);
            return;
        }
    }
}

void StartDetection(const std_msgs::Float32& msg)
{
    PublishToMotors(1); //MoveForward
    wallDistance = msg.data;
    isDetectingWall = true;
}

int main(int argc, char** argv) {
    //Node init
    ros::init(argc, argv, "wall_detector");
    ros::NodeHandle nh;

    //Subs
    ros::Subscriber lidar_sub = nh.subscribe("/scan", 10, lidarCallback);
    ros::Subscriber detectionActivation_sub = nh.subscribe("/startWallDetection", 10, StartDetection);

    //Pubs
    pubMotors = nh.advertise<geometry_msgs::Vector3>("/motorsControl", 10);
    pubWallFront = nh.advertise<std_msgs::Bool>("/frontWallDetection", 10);
    ros::Duration(2.0).sleep();
   
    //----


    //----
    ros::spin();
    return 0;
}
