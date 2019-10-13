#include "ros/ros.h"
#include "basic_rtos/rtman.h"


bool monitor(basic_rtos::rtman::Request &req,basic_rtos::rtman::Response &res)
{
	if(req.duration > 1){
	ROS_ERROR("TASK TOOK TOO LONG! DURATION: %f", req.duration);
	res.code = 99;
	return true;
	}else {
	ROS_INFO_STREAM("Task executed in " << req.duration << "s");
	res.code = 1;
        return true;
	}
	
}


int main(int argc, char **argv) {
 ros::init(argc, argv, "rtman");
 ros::NodeHandle n;

 ros::ServiceServer service = n.advertiseService("rt_manager", monitor);
 ROS_INFO("Monitoring task execution times...");
 ros::spin();

 return 0;
}

