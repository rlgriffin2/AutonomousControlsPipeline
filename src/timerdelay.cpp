#include "ros/ros.h"
#include "realtime/Delayed.h"
#include "realtime/Message.h"
#include <ctime>

//This will interact with catchdelay, so whenever catchdelay sends to mainoutnode,
//it will also send something to here. Everytime this node receives that, it will restart a timer
//and will constantly be publishing how long has elapsed since that last message sent
//so that catchdelay can know if X millseconds have passed

long startTime = 0;
bool publish = false;
float maxDelay = 80;
realtime::Delayed delayMsg;
void timerFunc(const realtime::Message::ConstPtr &msg)
{
    publish = true;
}

/*
    NEED TO HAVE THIS NODE TRACK THE DELAY IN MESSAGES, AND IF IT COMES IN ON TIME THEN SEND TRUE TO CATCHDELAY, AND IF NOT THEN SEND FALSE.
*/
int main(int argc, char **argv)
{
    ros::init(argc, argv, "n_timerdelay");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe<realtime::Message>("t_actual_data", 1, timerFunc);
    ros::Publisher pub = n.advertise<realtime::Delayed>("t_delay", 1);
    startTime = clock();
    while (ros::ok())
    {
        realtime::Delayed delayMsg;
        delayMsg.delayed = true;
        if (publish && (clock() - startTime) / (CLOCKS_PER_SEC / 1000) <= maxDelay)
        {
            startTime = clock();
            delayMsg.delayed = false;
            pub.publish(delayMsg);
            publish = false;
        }
        else if ((clock() - startTime) / (CLOCKS_PER_SEC / 1000) >= maxDelay)
        {
            startTime = clock();
            delayMsg.delayed = true;
            pub.publish(delayMsg);
        }
        ros::spinOnce();
    }
}