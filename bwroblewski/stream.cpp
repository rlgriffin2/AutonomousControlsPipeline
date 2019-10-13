#include "ros/ros.h"
#include "std_msgs/UInt32.h"

int main(int argc, char **argv) {
	ros::init(argc, argv, "stream");
	ros::NodeHandle n;
	ros::Publisher stream_pub = n.advertise<std_msgs::UInt32>("stream", 1000);
	// set publishing rate
	ros::Rate loop_rate(10);

	for (uint i = 0; ros::ok(); i++) {
		std_msgs::UInt32 num;
		num.data = i;
		if (rand() % 100 < 15) {
			ROS_INFO("%u SENT", num.data);
			stream_pub.publish(num);
		} else {
			ROS_INFO("%u DROPPED", num.data);
		}
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
