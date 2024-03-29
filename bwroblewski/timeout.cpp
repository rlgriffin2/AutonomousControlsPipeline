#include "ros/ros.h"
#include "std_msgs/UInt32.h"

uint timer = 0;
bool start = false;

void callback(const std_msgs::UInt32::ConstPtr& msg) {
	ROS_INFO("Recieved %u after %ums delay", msg->data, timer);
	start = true;
	timer = 0;
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "timeout");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("stream", 1000, callback);
	ros::Rate loop_rate(1000);
	while (ros::ok()) {
		timer++;
		if (start && timer == 5000) {
			ROS_ERROR("%us with no input from stream, terminating", timer / 1000);
			return 0;
		} else if (start && timer % 1000 == 0) {
			ROS_ERROR("%us with no input from stream", timer / 1000);
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
