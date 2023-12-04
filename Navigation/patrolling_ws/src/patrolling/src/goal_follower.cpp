#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"

using namespace std::chrono_literals;

// Declare the variables as constants for better readability
constexpr double x_start = -1;
constexpr double y_start = 3;
constexpr double z_start = 0;
constexpr double w_start = 1;

double x, y, z, or_x, or_y, or_z, or_w, v_x, v_y, v_z, w_x, w_y, w_z;
int t = 0;

void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
 x = msg->pose.pose.position.x;
 y = msg->pose.pose.position.y;
 z = msg->pose.pose.position.z;
 or_x = msg->pose.pose.orientation.x;
 or_y = msg->pose.pose.orientation.y;
 or_z = msg->pose.pose.orientation.z;
 or_w = msg->pose.pose.orientation.w;
 v_x = msg->twist.twist.linear.x;
 v_y = msg->twist.twist.linear.y;
 v_z = msg->twist.twist.linear.z;
 w_x = msg->twist.twist.angular.x;
 w_y = msg->twist.twist.angular.y;
 w_z = msg->twist.twist.angular.z;
}

int main(int argc, char * argv[]) {
 rclcpp::init(argc, argv);
 auto node = rclcpp::Node::make_shared("publisher");
 auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("/goal_pose", 10);
 auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, odom_callback);

 geometry_msgs::msg::PoseStamped message;
 message.pose.position.x = x_start;
 message.pose.position.y = y_start;
 message.pose.position.z = z_start;
 message.pose.orientation.x = 0;
 message.pose.orientation.y = 0;
 message.pose.orientation.z = 0;
 message.pose.orientation.w = w_start;

 rclcpp::WallRate loop_rate(500ms);
 rclcpp::WallRate poses(4);

 while (true) {
    publisher->publish(message);
    rclcpp::spin_some(node);
    loop_rate.sleep();
    t++;

    if (v_x < 0.005 && v_y < 0.005 && t >= 100) {
      poses.sleep();
      break;
    }
 }

 rclcpp::shutdown();
 return 0;
}

