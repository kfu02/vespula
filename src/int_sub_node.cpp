
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
  // TODO: ensure this can only be called once per thread, make private + put in
  // the constructor
  std::thread loop_thread([this]() {
    // TODO: replace w/ limited loop
    while (true) {
      if (kill_signal_) {
        return;
      }

      // TODO: need to lock weak_ptr to get shared_ptr, see int_pub_node.cpp
      std::shared_ptr<moodycamel::ReaderWriterQueue<int>> shared_sub_queue =
          sub_queue_.lock();

      if (!shared_sub_queue) {
        printf("ERROR: SUB QUEUE INVALID");
        break;
      }

      int message;
      bool new_message_arrived = shared_sub_queue->try_dequeue(message);
      if (new_message_arrived) {
        received_.push_back(message);
        // std::cout << name_ << " received: " << std::to_string(message) << std::endl;
      }

      std::this_thread::sleep_for(
          std::chrono::milliseconds((int)(1000.0 / tick_rate_)));
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