#include "ros/ros.h"
#include "std_msgs/String.h"
#include "basic_rtos/rtman.h"
#include "basic_rtos/delay.h"

#include <sstream>

int main(int argc, char **argv) 
{

 ros::init(argc, argv, "pub");

 ros::NodeHandle n;
  
 ros::Publisher p = n.advertise<std_msgs::String>("thread", 1);

 ros::ServiceClient rt = n.serviceClient<basic_rtos::rtman>("rt_manager");
 ros::ServiceClient delay = n.serviceClient<basic_rtos::delay>("delay_node");

 basic_rtos::rtman srv1;
 basic_rtos::delay srv2;

 ros::Time start;
 ros::Time end;
 ros::Duration d;
 double duration;


 ros::Rate loop_rate(10);

 int count =0;
 while(ros::ok()) {
    start = ros::Time::now();
    std_msgs::String msg;

    std::stringstream ss;
    ss << "message " << count;
    msg.data = ss.str();

    ROS_INFO("Sending: %s", msg.data.c_str());

    
    p.publish(msg);

    
	
    srv2.request.delay = 0.0;
    if(!delay.call(srv2)) {ROS_ERROR("ERROR IN DELAY SERVICE");}
    end = ros::Time::now();
    //ROS_INFO_STREAM("start time: " << start.toSec() << " end time: " << end.toSec());
    d = (end - start);
    srv1.request.duration = d.toSec();
    if(rt.call(srv1)){
	if(srv1.response.code == 99) {
	   ROS_ERROR("ABORTING TASK");
           break;
	}
    }else {
	ROS_ERROR("SYSTEM MONITOR OFFLINE");
    }
   
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
    

 }

 return 0;
}
