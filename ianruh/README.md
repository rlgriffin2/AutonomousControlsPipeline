Does not rely on any custom message types. For each topic it is monitoring, you define a frequency and a tolerance. Every time it receives a message, it calls a callback that updates the time in ms that the last message for that topic was received. In a separate thread it checks every 10ms every topic to determine if 1.) The message has or still can come in one time 2.) Is late, but is within the defined tolerance for that topic, or 3.) Is late and outside of the defined tolerance.



*Bug: I couldn't get `boost::bind` to work for me, so the callback isn't quite generalized. Once that works though, this should be able to support monitoring of an arbitrary number of topics.