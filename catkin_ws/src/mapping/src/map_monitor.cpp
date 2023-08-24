#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <std_srvs/Empty.h>
#include <string>

class MapMonitor {
public:
    MapMonitor() : last_map_size_(0), unchanged_counter_(0) {
        // Abonnement au topic de la carte
        map_sub_ = nh_.subscribe("/map", 10, &MapMonitor::mapCallback, this);
    }

    void mapCallback(const nav_msgs::OccupancyGrid::ConstPtr& msg) {
        int current_map_size = msg->data.size();

        if (current_map_size == last_map_size_) {
            unchanged_counter_++;
        } else {
            unchanged_counter_ = 0;
        }

        last_map_size_ = current_map_size;

        // Si la taille de la carte n'a pas changé pendant 10 cycles consécutifs, sauvegardez la carte
        if (unchanged_counter_ >= 10) {
            saveMap();
            ros::shutdown();
        }
    }

    void saveMap() {
        std::string save_path = "~/robotpix";
        std::string command = "rosrun map_server map_saver -f " + save_path;
        system(command.c_str());
        ROS_INFO("Map saved to %s", save_path.c_str());
    }

private:
    ros::NodeHandle nh_;
    ros::Subscriber map_sub_;
    int last_map_size_;
    int unchanged_counter_;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "map_monitor");
    MapMonitor monitor;
    ros::spin();
    return 0;
}
