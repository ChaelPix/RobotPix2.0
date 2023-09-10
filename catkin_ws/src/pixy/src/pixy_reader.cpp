#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/String.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <zbar.h>


ros::Publisher pubLcd;
zbar::ImageScanner scanner;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
     ROS_INFO("msg received");
    try
    {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
        cv::Mat cv_image = cv_ptr->image;

        cv::cvtColor(cv_image, cv_image, cv::COLOR_BGR2GRAY);
        zbar::Image image(cv_image.cols, cv_image.rows, "Y800", cv_image.data, cv_image.cols * cv_image.rows);
        ROS_INFO("Converted to ZBar image");
        scanner.scan(image);
        
        int num_symbols = std::distance(image.symbol_begin(), image.symbol_end());
        ROS_INFO("Detected %d symbols", num_symbols);


        // Traiter chaque symbole détecté
        for(auto symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
            if(symbol->get_type() == zbar::ZBAR_QRCODE) {
                std_msgs::String msg;
                msg.data = symbol->get_data();
                pubLcd.publish(msg); 
               ROS_INFO("Message sent: %s", msg.data.c_str());
            }
        }
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "qr_detector_node");
    ros::NodeHandle nh;

    ROS_INFO("START");

    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    pubLcd = nh.advertise<std_msgs::String>("/setLcdText", 10);

    // Charger l'image avec OpenCV
    cv::Mat image = cv::imread("/home/robot/robotPix/catkin_ws/src/pixy/executables/out.ppm", cv::IMREAD_COLOR); 

    if (image.empty()) {
        ROS_ERROR("Impossible de charger l'image.");
        return -1;
    }

    // Convertir l'image OpenCV en sensor_msgs/Image
    cv_bridge::CvImage cv_image;
    cv_image.encoding = "bgr8";
    cv_image.image = image;

    sensor_msgs::ImagePtr ros_image_ptr = cv_image.toImageMsg();
     ROS_INFO("Image ok, waiting");
    ros::Duration(2.0).sleep();

 ROS_INFO("Push Image");
    imageCallback(ros_image_ptr);

    ros::spin();
    return 0;
}