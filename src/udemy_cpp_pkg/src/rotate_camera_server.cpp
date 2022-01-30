#include "ros/ros.h"
#include "udemy_cpp_pkg/TurnCamera.h"
#include <ros/package.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>

bool sendImage(udemy_cpp_pkg::TurnCamera::Request &req,
               udemy_cpp_pkg::TurnCamera::Response &res)
{
  int angle_deg = req.angle;
  std::string img_path = ros::package::getPath("udemy_cpp_pkg") +
                         "/data/images/" + std::to_string(angle_deg) + ".png";
  // std::cout << img_path << '\n';
  // ROS_INFO("Image path: " + img_path);

  // check if we have requested images
  int angles_array[] = { -30, -15, 0, 15, 30 }; // supported angles
  bool angle_is_valid = false;
  for(int i = 0; i < sizeof(angles_array); i++){
       if(angles_array[i] == angle_deg){
           angle_is_valid = true;
       }
  }
  if (angle_is_valid)
  {
    ROS_INFO("Rotating the camera...");
    cv::Mat img = cv::imread(img_path, 1);
    sensor_msgs::ImagePtr img_ptr;
    img_ptr = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toImageMsg();
    res.image = *img_ptr;
  }
  else
  {
    ROS_ERROR("Can not rotate camera to this angle");
    return false;
  }

  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rotate_camera_server_node");

  ros::NodeHandle node_handle;

  // name of service and cb function as input params
  ros::ServiceServer service = node_handle.advertiseService("rotate_camera", sendImage);

  ROS_INFO("Trun Camera Server is Running...");
  ros::spin();

  return 0;
}
