#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include "beginner_tutorials/PipeInfo.h"
#include <ctime>

ros::Publisher output;
ros::Publisher info;

std::clock_t prev;
uint count = 0;

void pipeCallback(const std_msgs::String::ConstPtr& msg) {
	output.publish(msg);
	count++;
	beginner_tutorials::PipeInfo infoMsg;
	infoMsg.name = ros::this_node::getName();
	infoMsg.freq = ((double) (std::clock() - prev) / CLOCKS_PER_SEC);
	prev = std::clock();
	info.publish(infoMsg);
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "pipe");
	ros::NodeHandle n;
	ros::Subscriber input = n.subscribe("pipeIn", 1, pipeCallback);
	output = n.advertise<std_msgs::String>("pipeOut", 1);
	info = n.advertise<beginner_tutorials::PipeInfo>("pipeInfo", 1);
	prev = std::clock();
	ros::spin();
}
