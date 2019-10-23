#include "ros/ros.h"
#include "beginner_tutorials/PipeInfo.h"

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void callback(const beginner_tutorials::PipeInfo::ConstPtr& msg)
{
  ROS_INFO("name: [%s] freq: [%f]", msg->name.c_str(), msg->freq);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pipe_listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("pipeInfo", 1, callback);
  ros::spin();
  return 0;
}
