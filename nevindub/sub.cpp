#include "ros/ros.h"
#include "std_msgs/String.h"

void threadCallback(const std_msgs::String::ConstPtr& msg)
{
 ROS_INFO_STREAM("Receiving : " << msg->data.c_str());
}

int main(int argc, char **argv)
{

 ros::init(argc, argv, "sub");
 ros::NodeHandle n;

 ros::Subscriber s = n.subscribe("thread", 1, threadCallback);

 ros::spin();

 return 0;


}
