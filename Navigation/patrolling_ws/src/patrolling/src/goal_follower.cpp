#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"

using namespace std::chrono_literals;

double x,y,z,or_x,or_y,or_z,or_w, v_x,v_y,v_z,w_x,w_y,w_z;
int t=0;

void odom_callback(nav_msgs::msg::Odometry::SharedPtr msg){
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

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("publisher");
  auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("/goal_pose", 10);
  auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10,odom_callback);

  geometry_msgs::msg::PoseStamped message;
  nav_msgs::msg::Odometry odometry;
  rclcpp::WallRate loop_rate(500ms);
  rclcpp::WallRate poses(4);
  message.pose.position.x=-1;
  message.pose.position.y=3;
  message.pose.position.z=0;
  message.pose.orientation.x=0;
  message.pose.orientation.y=0;
  message.pose.orientation.z=0;
  message.pose.orientation.w=1;





  while (v_x > 0.001 || v_y > 0.001 || v_z > 0.001 || w_x > 0.0010 || w_y > 0.0010 || w_z > 0.001 ||t<=100) {
    
    publisher->publish(message);
    rclcpp::spin_some(node);
    loop_rate.sleep();
    t++;
    if(v_x<0.005 && v_y<0.005 && t==100){
      poses.sleep();
      break;
    }
  }
  rclcpp::shutdown();
  return 0;
}