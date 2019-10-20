// ############################
// DISCLAIMER
//
// Very ugly C++.
//
// This code is known to cause
// cancer in the state of
// California.
//
// ############################

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <string.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <boost/bind.hpp>

using namespace std::chrono;

// Struct defines requirments for each topics
typedef struct {
  int frequency;            // How much between messages ideally.
  int tolerance;            // How long until considered late
  int id;                   // Topic id
  milliseconds last_msg;    // Time of last message
  bool late;                // Store if it's already been identified as late
} TopicReq;

// Shouldn't probably be global, but I'm bad at C++
TopicReq** topics;

// Cal back for topic
void recievedMessage(const std_msgs::String::ConstPtr& msg, int channel) {
  // ROS_INFO("Message : [%s]", msg->data.c_str());
  milliseconds currentTime = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
  topics[channel]->last_msg = currentTime;
}

/* Get the status of the given topic
 * Return 0 if on time.
 * Return 1 if late, but within tolerance.
 * Return 2 if late and out of tolerance.
 */
int status(int topic_num) {

  // Before the client has started
  if(topics[topic_num]->last_msg.count() == 0) {
    return 0;
  }

  // Check how long it has been 
  milliseconds currentTime = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
  int delay = currentTime.count() - topics[topic_num]->last_msg.count();

  // Return accordingly
  if(delay > topics[topic_num]->frequency + topics[topic_num]->tolerance) {
    return 2;
  } else if(delay > topics[topic_num]->frequency) {
    return 1;
  }
  return 0;
}

// Seperateley threaded monitor from callbacks
void monitor(int numChannels) {

  // Loop while ros is alive
  while(ros::ok()) {
    // Go through every topic
    for(int i = 0; i < numChannels; i++) {
      int stat = status(i); // get Status

      // Respond however
      if(stat == 2) {
        // Late
        ROS_INFO("Client %i is late.\n", topics[i]->id);
      } else if(stat == 1) {
        // Over tolerance
        ROS_INFO("Client %i is over tolerance.\n", topics[i]->id);
      }
    }

    // Sleep for 10 milliseconds
    std::this_thread::sleep_for(milliseconds(10));
  }
}

// Main thread
int main(int argc, char **argv) {

  ros::init(argc, argv, "monitor");

  if(argc != 2) {
    fprintf(stderr, "Usage: [node] [num_channels]\n");
    exit(1);
  }

  int numChannel = strtol(argv[1], NULL, 10);

  topics = (TopicReq**)malloc(numChannel * sizeof(TopicReq*));

  for(int i = 0; i < numChannel; i++) {
    topics[i] = (TopicReq*)malloc(sizeof(TopicReq));
    topics[i]->frequency = 200; // in ms
    topics[i]->tolerance = 100; // in ms
    topics[i]->id = i;
    topics[i]->last_msg = milliseconds(0);
    topics[i]->late = false;
  }

  ros::NodeHandle n;

  for(int i = 0; i < numChannel; i++) {
    ros::Subscriber sub = n.subscribe<std_msgs::String>("topic" + std::to_string(i), 1, boost::bind(recievedMessage, _1, i));
  }
  // ros::Subscriber sub = n.subscribe("topic0", 1,recievedMessage_Topic);

  // Start minitor thread
  std::thread monitor_thread(monitor, numChannel);

  ros::spin();

  // Free mem
  for(int i = 0; i < numChannel; i++) {
    free(topics[i]);
  }
  free(topics);

  return 0;
}