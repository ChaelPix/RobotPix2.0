#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include "libpixyusb2.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pixy2_node");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("/image_raw", 1);

    Pixy2 pixy; // Utilisez Pixy2 au lieu de Link2USB

    if (pixy.init() != 0)
    {
        ROS_ERROR("Failed to initialize Pixy2");
        return -1;
    }

    return 0;
}
