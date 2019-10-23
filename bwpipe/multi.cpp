#include "ros/ros.h"
#include "std_msgs/String.h"

ros::Publisher output;

void callbackA(const std_msgs::String::ConstPtr& msg) {
	ROS_INFO("A: %s", msg->data.c_str());
}
void callbackB(const std_msgs::String::ConstPtr& msg) {
	ROS_INFO("B: %s", msg->data.c_str());
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "Multi");
	ros::NodeHandle n;
	ros::Subscriber a = n.subscribe("multiInA", 1, callbackA);
	ros::Subscriber b = n.subscribe("multiInB", 1, callbackB);
	ros::spin();
}
