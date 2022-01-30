#include "actionlib/server/simple_action_server.h"
#include "udemy_cpp_pkg/Navigate2DAction.h"
#include <cmath>

typedef actionlib::SimpleActionServer<udemy_cpp_pkg::Navigate2DAction> NavServer;


float DIST_THRESHOLD = 0.1;
geometry_msgs::Point robot_current_position;

void navCallback(const udemy_cpp_pkg::Navigate2DGoal::ConstPtr& goal_point,
                 NavServer* nav_server)
{
  udemy_cpp_pkg::Navigate2DFeedback feedback_msg;
  udemy_cpp_pkg::Navigate2DResult result_msg;

  ros::Rate feedback_rate(2);

  ROS_INFO("Goal recieved");
  double goal_recieved_time = ros::Time::now().toSec();

  float dist = std::sqrt(std::pow(robot_current_position.x - goal_point->point.x, 2) +
                         std::pow(robot_current_position.y - goal_point->point.y, 2));

  while(dist > DIST_THRESHOLD)
  {
    dist = std::sqrt(std::pow(robot_current_position.x - goal_point->point.x, 2) +
                     std::pow(robot_current_position.y - goal_point->point.y, 2));
    feedback_msg.distance_to_point = dist;

    nav_server->publishFeedback(feedback_msg);

    feedback_rate.sleep();
  }

  double goal_reached_time = ros::Time::now().toSec();
  ROS_INFO("Goal reached");

  double elapsed_time = goal_reached_time - goal_recieved_time;

  result_msg.elapsed_time = elapsed_time;
  nav_server->setSucceeded(result_msg);
}

void subscriberCallback(const geometry_msgs::Point::ConstPtr& msg)
{
  robot_current_position = *msg;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "navigate_2d_server_node");

  ros::NodeHandle node_handle;

  // subscriber to robot pose
  ros::Subscriber robot_position_sub = node_handle.subscribe("robot_position", 10, subscriberCallback);

  // bind to callback from boost library, and configure input arguments to cb, lasr argument false - autostart cb
  NavServer server(node_handle, "navigate_2d", boost::bind(&navCallback, _1, &server), false);
  // start server manually
  server.start();

  ros::spin();

  return 0;
}
