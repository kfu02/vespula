#include "int_node.h"

IntPublisherNode::IntPublisherNode(std::string name, int tick_rate,
                                   Topic<int> &pub_topic) {
  // TODO: initializer list here
  name_ = name;
  tick_rate_ = tick_rate;
  pub_queue_ = pub_topic.add_publisher(*this);
}

IntPublisherNode::~IntPublisherNode() {
  // free(pub_queue_);
}

const std::string &IntPublisherNode::name() const { return name_; }

const int &IntPublisherNode::tick_rate() const { return tick_rate_; }

void IntPublisherNode::setup() {}

std::thread IntPublisherNode::loop() {
  // TODO: ensure this can only be called once per thread, make private + put in
  // the constructor
  std::thread loop_thread([this]() {
    int i = 0;
    while (i < 10) {
      std::shared_ptr<moodycamel::ConcurrentQueue<int>> shared_pub_queue =
          pub_queue_.lock();

      if (!shared_pub_queue) {
        // queue no longer valid
        printf("invalid queue\n");
        break;
      }

      printf("enqueue %d\n", i);
      shared_pub_queue->enqueue(i);
      // int out = 0;
      // shared_pub_queue->try_dequeue(out);
      // printf("out: %d\n", out);
      i++;

      std::this_thread::sleep_for(
          std::chrono::milliseconds((int)(1000.0 / tick_rate_)));
    }
  });
  return loop_thread;
}
