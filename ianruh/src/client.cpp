#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

  if(argc != 3) {
    fprintf(stderr, "Usage: [node] [client/topic #] [drop_rate [0-999]]");
    exit(1);
  }

  int clientNum = strtol(argv[1], NULL, 10);
  int dropRate = strtol(argv[2], NULL, 10);

  ros::init(argc, argv, "client" + std::to_string(clientNum));
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<std_msgs::String>("topic" + std::to_string(clientNum), 1);

  ros::Rate loop_rate(5); // 5 times per second (200ms delay)

  int count = 0;
  while (ros::ok()) {
    std_msgs::String msg;

    std::stringstream ss;
    ss << "hello world " << count;
    msg.data = ss.str();

    bool should_skip = (rand()%1000 < dropRate) ? true : false;

    if(!should_skip) {
      pub.publish(msg);
    } else {
      ROS_INFO("Skipped\n");
    }

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }

  return 0;
}