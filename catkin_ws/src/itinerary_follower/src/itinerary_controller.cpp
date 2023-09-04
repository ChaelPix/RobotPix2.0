// #include <ros/ros.h>
// #include <move_base_msgs/MoveBaseAction.h>
// #include <actionlib/client/simple_action_client.h>
// #include <geometry_msgs/PoseStamped.h>

// typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

// int main(int argc, char** argv){
//   ros::init(argc, argv, "itineraryController");

//   // Créer l'action client
//   // MoveBaseClient ac("move_base", true);

//   // // Attendre que l'action serveur soit prête
//   // while(!ac.waitForServer(ros::Duration(5.0))){
//   //   ROS_INFO("En attente du serveur d'action move_base...");
//   // }

//   // move_base_msgs::MoveBaseGoal goal;

//   // // Définir l'objectif
//   // goal.target_pose.header.frame_id = "map";
//   // goal.target_pose.header.stamp = ros::Time(0);
//   // goal.target_pose.pose.position.x = 1.0;
//   // goal.target_pose.pose.position.y = 1.0;
//   // goal.target_pose.pose.orientation.w = 1.0;

//   // ROS_INFO("Envoi de l'objectif");
//   // ac.sendGoal(goal);

//   // ac.waitForResult();

//   // if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
//   //   ROS_INFO("Le robot a atteint l'objectif");
//   // else
//   //   ROS_INFO("Le robot n'a pas pu atteindre l'objectif");

//   // return 0;

//    ros::NodeHandle nh;

//     ros::Publisher pub = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 10);
//     ros::Duration(2.0).sleep();
//     geometry_msgs::PoseStamped target;
//     target.header.frame_id = "map";
//     target.pose.position.x = 1.0;
//     target.pose.position.y = 1.0;
//     target.pose.orientation.w = 1.0;

//     pub.publish(target);

//     ros::spin();
//     return 0;
// }
// #include <ros/ros.h>
// #include <actionlib/client/simple_action_client.h>
// #include <nav2d_navigator/MoveToPosition2DAction.h>

// int main(int argc, char **argv)
// {
//     ros::init(argc, argv, "autonomous_navigation");
//     ros::NodeHandle nh;

//     actionlib::SimpleActionClient<nav2d_navigator::MoveToPosition2DAction> ac("MoveToPosition2D", true);

//     ROS_INFO("En attente du serveur d'action...");
//     ac.waitForServer();

//     nav2d_navigator::MoveToPosition2DGoal goal;
//     goal.target_pose.x = 1.0; // Coordonnée X de l'objectif
//     goal.target_pose.y = 1.0; // Coordonnée Y de l'objectif
//     goal.target_pose.theta = 0.0; // Orientation de l'objectif
//     goal.header.frame_id = "map"; // Le cadre de référence de l'objectif

//     ROS_INFO("Envoi de l'objectif de navigation.");
//     ac.sendGoal(goal);

//     ROS_INFO("En attente du résultat...");
//     ac.waitForResult();

//     if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
//         ROS_INFO("L'objectif a été atteint !");
//     else
//         ROS_INFO("La navigation a échoué.");

//     return 0;
// }

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>

void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    double robot_x = msg->pose.position.x;
    double robot_y = msg->pose.position.y;

    ROS_INFO("x: %f, y: %f", robot_x, robot_y);
    // ou en utilisant std::to_string
    // ROS_INFO(("x: " + std::to_string(robot_x) + ", y: " + std::to_string(robot_y)).c_str());

    // Ici, vous pouvez implémenter votre logique pour déplacer le robot vers les waypoints.
    // Par exemple, si le robot est assez proche du waypoint courant, passer au suivant.
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "itineraryController");
    ros::NodeHandle nh;

    ros::Subscriber pose_sub = nh.subscribe("/slam_out_pose", 1000, poseCallback);

    ros::spin();

    return 0;
}
