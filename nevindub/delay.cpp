#include "ros/ros.h"
#include "basic_rtos/delay.h"


bool addDelay(basic_rtos::delay::Request &req,basic_rtos::delay::Response &res)
{
	ROS_INFO_STREAM("Delaying task by " << req.delay << "s");
        ros::Duration(req.delay).sleep(); 
	return true;
}


int main(int argc, char **argv) {
 ros::init(argc, argv, "delay");
 ros::NodeHandle n;

 ros::ServiceServer service = n.advertiseService("delay_node", addDelay);
 ros::spin();

 return 0;
}

