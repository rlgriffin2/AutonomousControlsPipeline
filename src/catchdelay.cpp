#include "ros/ros.h"
#include "realtime/Message.h"
#include "realtime/Delayed.h"
#include <ctime>

//Subscribed to t_delay, publishes to t_selected_data when message from t_actual_data isn't received in time window (measured by t_delay)

bool delayed = true; //assume message to be delayed
realtime::Message receivedInfo;
void subFunc(const realtime::Message::ConstPtr &msg)
{
	receivedInfo.info = msg->info;
}

void delayFunc(const realtime::Delayed::ConstPtr &delayMsg)
{
	delayed = delayMsg->delayed;
}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "n_catchdelay");
	ros::NodeHandle n;
	realtime::Message fillerInfo;
	fillerInfo.info = "Filler message, real message delayed";
	ros::Subscriber data_sub = n.subscribe<realtime::Message>("t_actual_data", 1, subFunc);
	ros::Subscriber delay_sub = n.subscribe<realtime::Delayed>("t_delay", 1, delayFunc);
	ros::Publisher data_pub = n.advertise<realtime::Message>("t_selected_data", 1);
	ros::Rate loop_rate(9);
	//ros::Publisher delay_pub = n.advertise<realtime::Message>("t_delay_topic", 10);
	while (ros::ok())
	{
		if (!delayed)
		{ //probably don't even need the received thing
			data_pub.publish(receivedInfo);
		}
		else
		{
			data_pub.publish(fillerInfo);
		}
		loop_rate.sleep();

		ros::spinOnce();
	}
}
