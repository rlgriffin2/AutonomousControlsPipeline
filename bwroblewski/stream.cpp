#include "ros/ros.h"
#include "std_msgs/UInt32.h"

int main(int argc, char **argv) {
	ros::init(argc, argv, "stream");
	ros::NodeHandle n;
	ros::Publisher stream_pub = n.advertise<std_msgs::UInt32>("stream", 1000);
	// set publishing rate
	ros::Rate loop_rate(10);

	for (uint i = 0; i < 100; i++) {
		std_msgs::UInt32 num;
		num.data = i;
		ROS_INFO("%u", num.data);
		stream_pub.publish(num);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
