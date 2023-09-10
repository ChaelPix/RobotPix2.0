#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <cstdlib>  

#include <fstream>
#include <sstream>
#include <string>
#include <vector>


/*--------------------------*/
enum class ImgType {Color, Bar, Qr};
ImgType imgType = ImgType::Qr;
/*--------------------------*/
ros::Publisher pubLcd;
zbar::ImageScanner scanner;
int nbErrors = 0;
bool isRunning = false;

std::vector<int> extractBlockNumbers(const std::string& filename) {
    std::vector<int> blockNumbers;
    std::ifstream file(filename);

    if (!file.is_open()) {
        return blockNumbers;  // retourner un vecteur vide en cas d'erreur d'ouverture du fichier
    }

    std::string line;
    while (std::getline(file, line)) {
        // Chercher une ligne qui commence par "Block"
        if (line.find("Block") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            int blockNumber;

            // Ignore les mots "Block" et "Data"
            iss >> temp >> blockNumber >> temp >> temp;
            blockNumbers.push_back(blockNumber);
        }
    }

    file.close();

    return blockNumbers;
}

int ReadColor()
{
    int result = system("/home/robot/robotPix/catkin_ws/src/pixy/executables/get_blocks");
    if (result != 0) {
        ROS_WARN("Failed to execute the external executable.");
    }

    std::vector<int> blockNumbers = extractBlockNumbers("/home/robot/robotPix/catkin_ws/src/pixy/executables/output.txt");
    for (int num : blockNumbers) {
        std::cout << "Detected Block Number: " << num << std::endl;
    }

    
    if (!blockNumbers.empty()) {
        std_msgs::String msg;
        msg.data = "Block #" + std::to_string(blockNumbers.at(0));
        pubLcd.publish(msg);
        return 1;
    } else {
        return -1;
    }
}

int ReadBarCode(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
        cv::Mat cv_image = cv_ptr->image;

        cv::cvtColor(cv_image, cv_image, cv::COLOR_BGR2GRAY);
        zbar::Image image(cv_image.cols, cv_image.rows, "Y800", cv_image.data, cv_image.cols * cv_image.rows);
        ROS_INFO("Converted to ZBar image");
        scanner.scan(image);

        // Traiter chaque symbole détecté
        for(auto symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
            // Pour détecter un code-barres, nous vérifions les types courants de codes-barres, comme EAN et CODE128.
            if(symbol->get_type() == zbar::ZBAR_EAN13 || symbol->get_type() == zbar::ZBAR_CODE128) {
                std_msgs::String msg;
                msg.data = symbol->get_data();
                pubLcd.publish(msg);
                ROS_INFO("Bar code : %s", msg.data.c_str());
                return 1;
            }
        }
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
        return -1;
    }
}

int ReadQrCode(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
        cv::Mat cv_image = cv_ptr->image;

        cv::cvtColor(cv_image, cv_image, cv::COLOR_BGR2GRAY);
        zbar::Image image(cv_image.cols, cv_image.rows, "Y800", cv_image.data, cv_image.cols * cv_image.rows);
        ROS_INFO("Converted to ZBar image");
        scanner.scan(image);
        
        int num_symbols = std::distance(image.symbol_begin(), image.symbol_end());


        // Traiter chaque symbole détecté
        for(auto symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
            if(symbol->get_type() == zbar::ZBAR_QRCODE) {
                std_msgs::String msg;
                msg.data = symbol->get_data();
                pubLcd.publish(msg); 
                ROS_INFO("Qr code : %s", msg.data.c_str());
                return 1;
            }
        }
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
        return -1;
    }
}

int TakePicture()
{
    int result = system("/home/robot/robotPix/catkin_ws/src/pixy/executables/get_raw_frame");
    if (result != 0) {
        ROS_WARN("Failed to execute the external executable.");
    }

    ros::Duration(2.0).sleep();

    cv::Mat image = cv::imread("/home/robot/robotPix/catkin_ws/src/pixy/executables/out.ppm", cv::IMREAD_COLOR); 
    cv_bridge::CvImage cv_image;
    cv_image.encoding = "bgr8";
    cv_image.image = image;

    sensor_msgs::ImagePtr ros_image_ptr = cv_image.toImageMsg();
    int isSuccess = 0;

    if(imgType == ImgType::Qr)
        isSuccess = ReadQrCode(ros_image_ptr);
    else
        isSuccess = ReadBarCode(ros_image_ptr);

    return isSuccess;
}

void Start(const std_msgs::Bool& msg)
{
    if(isRunning)
        return;

    isRunning = true;
    int isSuccess = -1;

    while (isSuccess < 0 && nbErrors < 3) {
        if(imgType == ImgType::Color) 
            isSuccess = ReadColor();
        else 
            isSuccess = TakePicture();
        
        if (isSuccess < 0) 
            nbErrors++;     
    }

    isRunning = false;
    nbErrors = 0; 
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pixy_node");
    ros::NodeHandle nh;

    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

    //Subs
    ros::Subscriber start_sub = nh.subscribe("/pixyTrigger", 10, Start);

    //Pubs
    pubLcd = nh.advertise<std_msgs::String>("/setLcdText", 10);

    ros::Duration(2.0).sleep();

    ros::spin();
    return 0;
}