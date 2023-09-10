#include <ros/ros.h>
#include <std_msgs/String.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <geometry_msgs/Vector3.h>

const float robotSpeed = 250;
const float delay = 0.5;
ros::Publisher pubMotors;
void PublishToMotors(int action)
{
    geometry_msgs::Vector3 robot_msg;
    robot_msg.x = robotSpeed;
    robot_msg.y = action;
    pubMotors.publish(robot_msg);

    ros::Duration(delay).sleep();
    robot_msg.y = 0;
    pubMotors.publish(robot_msg);
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "mouse_listener");
    ros::NodeHandle nh;
    ros::Publisher mouse_pub = nh.advertise<std_msgs::String>("mouse_events", 10);

    // Ouvrez le périphérique de la souris
    int fd = open("/dev/input/event0", O_RDONLY|O_NONBLOCK); // Remplacez "eventX" par le bon événement pour votre souris
    struct libevdev *dev = NULL;
    libevdev_new_from_fd(fd, &dev);
    pubMotors = nh.advertise<geometry_msgs::Vector3>("/motorsControl", 10);
    ros::Duration(1.0).sleep();
    
    while (ros::ok()) {
        struct input_event ev;
        int rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0) {
            std_msgs::String msg;

            if (ev.type == EV_REL && ev.code == REL_WHEEL) {
                if (ev.value > 0) {
                    msg.data = "Molette vers le haut";
                    PublishToMotors(6);
                }
                else {
                    msg.data = "Molette vers le bas";
                    PublishToMotors(7);
                }
            } else if (ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1) {
                msg.data = "Clic gauche";
                PublishToMotors(1);
            } else if (ev.type == EV_KEY && ev.code == BTN_RIGHT && ev.value == 1) {
                msg.data = "Clic droit";
                PublishToMotors(2);
            } else if (ev.type == EV_KEY && ev.code == BTN_EXTRA && ev.value == 1) {
                msg.data = "Bouton sur le cote 1";
                PublishToMotors(3);
            } else if (ev.type == EV_KEY && ev.code == BTN_SIDE && ev.value == 1) {
                msg.data = "Bouton sur le cote 2"; 
                PublishToMotors(4);
            }

            if (!msg.data.empty()) {
                mouse_pub.publish(msg);
                ROS_INFO("Mouse data: %s", msg.data.c_str()); 
            }
        }
    }

    libevdev_free(dev);
    close(fd);
    return 0;
}
