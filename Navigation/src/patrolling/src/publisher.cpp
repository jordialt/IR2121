#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"

using namespace std::chrono_literals;

double x,y,z,or_x,or_y,or_z,or_w;

void odom_callback(nav_msgs::msg::Odometry::SharedPtr msg){
  x = msg->pose.pose.position.x;
  y = msg->pose.pose.position.y;
  z = msg->pose.pose.position.z;
  or_x = msg->pose.pose.orientation.x;
  or_y = msg->pose.pose.orientation.y;
  or_z = msg->pose.pose.orientation.z;
  or_w = msg->pose.pose.orientation.w;

}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("publisher");
  auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("goal_pose", 10);
  auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10,odom_callback);

  geometry_msgs::msg::PoseStamped message;
  nav_msgs::msg::Odometry odometry;
  rclcpp::WallRate loop_rate(500ms);
  message.pose.position.x=2;
  message.pose.position.y=2;
  message.pose.position.z=0;
  message.pose.orientation.x=0;
  message.pose.orientation.y=0;
  message.pose.orientation.z=0;
  message.pose.orientation.w=1;



  while (x!=message.pose.position.x && y!=message.pose.position.y && z!=message.pose.position.z) {
    
    publisher->publish(message);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  return 0;
}