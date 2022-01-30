#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "geometry_msgs/Twist.h"


float WHEEL_RADIUS = 0.125;  // [meters]

ros::Publisher speed_pub;

void subCallback(const std_msgs::Float32ConstPtr& msg)
{
  ros::NodeHandle callback_node_handle;

  if (callback_node_handle.getParam("wheel_radius", WHEEL_RADIUS))
  {
    float rpm = msg->data;
    std_msgs::Float32 speed_msg;
    speed_msg.data = 2*3.1415 * rpm * WHEEL_RADIUS / 60.0;  // [meters/sec]

    ROS_INFO("Linear velocity: %f", speed_msg.data);
    speed_pub.publish(speed_msg);
  }
  else
  {
    ROS_WARN("No value set for wheel_radius parameter.");
  }

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rpm_sub_node");

  ros::NodeHandle node_handle;

  speed_pub = node_handle.advertise<std_msgs::Float32>("speed", 10);

  ros::Subscriber rpm_sub = node_handle.subscribe("rpm", 10, subCallback);

  ros::spin();

  return 0;
}
