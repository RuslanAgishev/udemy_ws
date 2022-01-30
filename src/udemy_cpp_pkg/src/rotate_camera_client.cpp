#include "ros/ros.h"
#include "udemy_cpp_pkg/TurnCamera.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rotate_camera_client_node");

  ros::NodeHandle node_handle;

  ros::ServiceClient client = node_handle.serviceClient<udemy_cpp_pkg::TurnCamera>("rotate_camera");

  udemy_cpp_pkg::TurnCamera rotate_camera_srv;

  std::cout << "Type 370 to quit" << std::endl;

  int des_angle;
  int supported_angles[] = { -30, -15, 0, 15, 30 }; // supported angles
  while (ros::ok)
  {
    std::cout << "Enter an angle in degrees from -30, -15, 0, 15, 30 options: ";
    std::cin >> des_angle;

    if (des_angle == 370)
    {
      ROS_INFO("Exiting application...");
      return 0;
    }

    // check if we have requested images
    bool angle_is_valid = false;
    for(int i = 0; i < sizeof(supported_angles); i++)
    {
         if(supported_angles[i] == des_angle)
         {
             angle_is_valid = true;
         }
    }
    if (angle_is_valid == false)
    {
      continue;
    }

    rotate_camera_srv.request.angle = des_angle;
    // if client successfully send request to a server or not
    if (client.call(rotate_camera_srv))
    {
      sensor_msgs::Image img_msg;
      img_msg = rotate_camera_srv.response.image;

      // visualize image here
      ROS_INFO("Showing image...");
      cv_bridge::CvImagePtr cv_ptr;
      cv_ptr = cv_bridge::toCvCopy(rotate_camera_srv.response.image,
                                   sensor_msgs::image_encodings::BGR8);
      cv::imshow("Robot camera image", cv_ptr->image);
      cv::waitKey(1000);
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
