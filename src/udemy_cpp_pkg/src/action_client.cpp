#include "actionlib/client/simple_action_client.h"
#include "udemy_cpp_pkg/Navigate2DAction.h"
#include <cmath>

typedef actionlib::SimpleActionClient<udemy_cpp_pkg::Navigate2DAction> NavClient;

bool feedback_cb_ready_flag = false;

void activeCallback()
{
  std::cout << "\nGoal Activated" << std::endl;
  feedback_cb_ready_flag = true;
}

void feedbackCallback(const udemy_cpp_pkg::Navigate2DFeedback::ConstPtr& feedback)
{
  if (feedback_cb_ready_flag)
  {
    // fancy printing
    std::cout << "\33[2K\r";
    std::cout << "Distance to goal: " << feedback->distance_to_point;
    std::cout.flush();
  }
}

void resultCallback(const actionlib::SimpleClientGoalState& state,
                    const udemy_cpp_pkg::Navigate2DResult::ConstPtr& result)
{
  std::cout << "\nFinished.\nTime elapsed: " << result->elapsed_time << std::endl;
  feedback_cb_ready_flag = false;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "navigate_2d_client_node");

  NavClient client("navigate_2d", true); // true to keep the node running
  client.waitForServer();

  // wait for input from user and set it as a goal
  udemy_cpp_pkg::Navigate2DGoal goal;
  goal.point.z = 0;

  while (ros::ok()) {
    std::cout << "\nEnter the X-coordinate: ";
    std::cin >> goal.point.x;

    std::cout << "\nEnter the Y-coordinate: ";
    std::cin >> goal.point.y;

    // inputs: goal msg, address of cb that processes the result,
    // the address of the cb that runs whenever the goal becomes active,
    // the address of the feedback cb function
    client.sendGoal(goal, &resultCallback, &activeCallback, &feedbackCallback);

    client.waitForResult(); // wait for results from server
  }

  return 0;

}
