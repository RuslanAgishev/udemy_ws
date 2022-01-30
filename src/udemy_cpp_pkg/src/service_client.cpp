#include "ros/ros.h"
#include "udemy_cpp_pkg/OddEvenCheck.h"

#include <iostream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "odd_even_service_client_node");

  ros::NodeHandle node_handle;

  ros::ServiceClient client = node_handle.serviceClient<udemy_cpp_pkg::OddEvenCheck>("odd_even_check");

  udemy_cpp_pkg::OddEvenCheck odd_even_check_srv;

  std::cout << "Type 0 to quit" << std::endl;

  int input_number;
  while (ros::ok)
  {
    std::cout << "Enter an integer: ";
    std::cin >> input_number;

    odd_even_check_srv.request.number = input_number;

    if (input_number == 0)
    {
      ROS_INFO("Exiting application...");
      return 0;
    }

    // if client successfulle send request to a server or not
    if (client.call(odd_even_check_srv))
    {
      std::string resp;
      resp = odd_even_check_srv.response.answer;

      std::cout << "Your input number is " << resp << std::endl;
    }
    else
    {
      ROS_ERROR("Service call failed");
      return 1;
    }

    // clear the terminal buffer
    std::cin.clear();
  }
}
