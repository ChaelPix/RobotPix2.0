#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Float32.h>

const float robotSpeed = 150;

//ros::Publisher wall_pub;
ros::Publisher pubMotorsStop;
ros::Publisher pubMotorsForward;

void lidarCallback(const sensor_msgs::LaserScan::ConstPtr& msg) {
    // Seuil de distance pour détecter un mur (par exemple, 0.5 mètre)
    const float WALL_DISTANCE_THRESHOLD = 0.5;

    // Intervalles d'angles pour détecter un mur devant (en radians)
    const float MIN_ANGLE = -15 * M_PI / 180;  // -15 degrés convertis en radians
    const float MAX_ANGLE = 15 * M_PI / 180;   // 15 degrés convertis en radians

    // Calculer les indices correspondants dans le tableau de mesures du LIDAR
    int min_index = (MIN_ANGLE - msg->angle_min) / msg->angle_increment;
    int max_index = (MAX_ANGLE - msg->angle_min) / msg->angle_increment;

  

    for (int i = min_index; i <= max_index; i++) {
        if (msg->ranges[i] < WALL_DISTANCE_THRESHOLD) {
            // Mur détecté devant
            // std_msgs::Bool wall_msg;
            // wall_msg.data = true;
            // wall_pub.publish(wall_msg);
            std::stringstream ss; // Créer un stringstream
            ss << "lidarCallback"; 
            ROS_INFO(ss.str().c_str()); 

            std_msgs::Empty empty_msg;
            pubMotorsStop.publish(empty_msg);
            return;
        }
    }
}

void MotorsCallback(const std_msgs::Bool& msg)
{
    std::stringstream ss; // Créer un stringstream
            ss << "Motors OK"; 
            ROS_INFO(ss.str().c_str()); 
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "wall_detector");
    ros::NodeHandle nh;

    ros::Subscriber lidar_sub = nh.subscribe("/scan", 10, lidarCallback);
    ros::Subscriber motors_sub = nh.subscribe("MotorsCallBack", 10, MotorsCallback);

    pubMotorsStop = nh.advertise<std_msgs::Empty>("/motorsStop", 10);
    pubMotorsForward = nh.advertise<std_msgs::Float32>("/motorsMoveForward", 10);

    ros::Duration(2.0).sleep();

    //wall_pub = nh.advertise<std_msgs::Bool>("/detected_wall", 10);
   
    std_msgs::Float32 speed_msg;
    speed_msg.data = robotSpeed;
    pubMotorsForward.publish(speed_msg);

    ros::spin();
    return 0;
}
