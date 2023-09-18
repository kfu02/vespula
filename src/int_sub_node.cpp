
#include "int_sub_node.h"

IntSubscriberNode::IntSubscriberNode(std::string name, int tick_rate,
                                   Topic<int> &sub_topic) {
  // TODO: initializer list here
  name_ = name;
  tick_rate_ = tick_rate;
  sub_queue_ = sub_topic.add_subscriber(*this);

  received_ = std::vector<int>();
}

IntSubscriberNode::~IntSubscriberNode() {
  // free(sub_queue_);
}

const std::string &IntSubscriberNode::name() const { return name_; }

const int &IntSubscriberNode::tick_rate() const { return tick_rate_; }

void IntSubscriberNode::setup() {
}

void IntSubscriberNode::loop() {
  std::thread loop_thread([this]() {
    int received = 0;
    while (true) {
      // Mark the time when the loop starts
      auto loopStartTime = std::chrono::high_resolution_clock::now();

      if (kill_signal_) {
        return;
      }

      std::shared_ptr<moodycamel::ConcurrentQueue<int>> shared_sub_queue =
          sub_queue_.lock();

      if (!shared_sub_queue) {
        continue;
      }

      int message[100];
      size_t ct = shared_sub_queue->try_dequeue_bulk(message, 100);
      // printf("received: %d\n", ct);
      // std::cout << "test" << std::endl;

      for (int i = 0; i < ct; i++) {
        received_.push_back(message[i]);
      }
      // if (new_message_arrived) {
        // received_.push_back(message);
        // received++;
      // }

      std::this_thread::sleep_for(
          std::chrono::milliseconds((int)(1000.0 / tick_rate_)));
      
      // Calculate how much time has elapsed since the start of the loop
      auto loopEndTime = std::chrono::high_resolution_clock::now();
      auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(loopEndTime - loopStartTime);

      // Check if at least 10 ms have elapsed
      if (elapsedMilliseconds < std::chrono::milliseconds((int)(1000.0 / tick_rate_))) {
          // Calculate how much time to sleep to reach 10 ms
          auto sleepDuration = std::chrono::milliseconds((int)(1000.0 / tick_rate_)) - elapsedMilliseconds;
          
          // Sleep for the remaining time
          std::this_thread::sleep_for(sleepDuration);
      } else {
        // printf("WARN: sub tick rate is too slow to keep up with the incoming messages!");
      }

    }
  });
  loop_thread.detach();
}

std::vector<int> &IntSubscriberNode::get_received() {
  return received_;
}

void IntSubscriberNode::kill() {
  kill_signal_ = true;
}
