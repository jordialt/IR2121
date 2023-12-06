#include <chrono>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"

using namespace std::chrono_literals;



double x, y, z, or_x, or_y, or_z, or_w, v_x, v_y, v_z, w_x, w_y, w_z;

bool isGoalReached(double threshold_velocity)
{
 return (v_x < threshold_velocity && v_y < threshold_velocity); 
} 
    


void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
 x = msg->pose.pose.position.x;
 y = msg->pose.pose.position.y;
 z = msg->pose.pose.position.z;

  v_x = msg->twist.twist.linear.x;
  v_y = msg->twist.twist.linear.y;
  w_x = msg->twist.twist.angular.x;
  w_y = msg->twist.twist.angular.y;


}

int main(int argc, char * argv[]) {
 rclcpp::init(argc, argv);
 auto node = rclcpp::Node::make_shared("publisher");
 auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("/goal_pose", 10);
 auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, odom_callback);

 std::vector<geometry_msgs::msg::PoseStamped> goals;
 geometry_msgs::msg::PoseStamped goal1;
 geometry_msgs::msg::PoseStamped goal2;
 geometry_msgs::msg::PoseStamped goal3;
 geometry_msgs::msg::PoseStamped goal4;

 goal1.pose.position.x=8;
 goal1.pose.position.y=-3;
 goal2.pose.position.x=2.5;
 goal2.pose.position.y=4;
 goal3.pose.position.x=-6;
 goal3.pose.position.y=-2;
 goal4.pose.position.x=-3;
 goal4.pose.position.y=5;

 goals={goal1,goal2,goal3,goal4};
 

 rclcpp::WallRate loop_rate(500ms);
 rclcpp::WallRate poses(4);

int i = 0;
int t = 0;

 while (i<int(goals.size())) {
    publisher->publish(goals[i]);
    rclcpp::spin_some(node);
    loop_rate.sleep();
    if(isGoalReached(0.005) and t>100 ){
      i++;
      t=0;
      
    }
    t++;

   
 }

 rclcpp::shutdown();
 return 0;
}
