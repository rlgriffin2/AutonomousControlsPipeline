#include "ros/ros.h"
#include "realtime/Message.h"
#include <time.h>

//Publishes to t_data, occasionally has a random delay for publishing data
realtime::Message normTimeMsg;
realtime::Message delayedMsg;
void delay(int numMS)
{
	clock_t startTime = clock();

	while (clock() / (CLOCKS_PER_SEC / 1000) < startTime / (CLOCKS_PER_SEC / 1000) + numMS) //delay function, just loops through the while loop blankly
	{
	};
}
int main(int argc, char **argv)
{
	std::string baseDMsg = "This message was delayed";
	std::string baseNMsg = "This message was not delayed";
	ros::init(argc, argv, "n_delaypublish");
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<realtime::Message>("t_actual_data", 1);
	srand(time(0));
	ros::Rate loop_rate(10);
	while (ros::ok())
	{
		int num = rand() % 5000;
		if (num > 1000)
		{
			normTimeMsg.info = baseNMsg + std::to_string(num);
			ROS_INFO("msg: %s", normTimeMsg.info.c_str());
			pub.publish(normTimeMsg);
		}
		else
		{
			delayedMsg.info = baseDMsg + std::to_string(num);
			ROS_INFO("msg: %s", delayedMsg.info.c_str());
			delay(1000);
			pub.publish(delayedMsg);
		}
		loop_rate.sleep();
		ros::spinOnce();
	}
}
