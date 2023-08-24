#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>

class MappingNode {
public:
    MappingNode() {
        pubMotors = nh.advertise<geometry_msgs::Vector3>("/motorsControl", 10);
        cmd_vel_sub_ = nh.subscribe("/move_base/cmd_vel", 10, &MappingNode::cmdVelCallback, this);
    }

    void cmdVelCallback(const geometry_msgs::Twist::ConstPtr& msg) {
        if (msg->linear.x > 0) {
            // Avancer
            PublishToMotors(1);
        } else if (msg->linear.x < 0) {
            // Rculer
            PublishToMotors(2);
        } else if (msg->angular.z > 0) {
            // Tourner à gauche
            PublishToMotors(3);
        } else if (msg->angular.z < 0) {
            // Tourner à droite
            PublishToMotors(4);
        }
    }

    void PublishToMotors(int action)
    {
        geometry_msgs::Vector3 robot_msg;
        robot_msg.x = robotSpeed;
        robot_msg.y = action;
        pubMotors.publish(robot_msg);
    }

private:
    ros::NodeHandle nh;
    ros::Subscriber cmd_vel_sub_;
    const float robotSpeed = 150;
    ros::Publisher pubMotors;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "mapping_node");
    MappingNode node;
    ros::spin();
    return 0;
}
