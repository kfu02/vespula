#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "concurrentqueue.h"
#include "readerwriterqueue.h"

#include "node.h"

template <typename T>

#define TOPIC_TICK_RATE 1000

class Topic {
private:
  // shared_ptr only to allow Nodes to receive weak_ptrs from this class, Topic
  // still owns the publisher queue
  std::shared_ptr<moodycamel::ConcurrentQueue<T>> publisher_queue_;

  std::vector<std::string> publisher_names_;

  std::vector<moodycamel::ReaderWriterQueue<T>> subscriber_queues_;
  std::vector<std::string> subscriber_names_;

  std::string topic_name_;
  bool debug_;

public:
  Topic(std::string topic_name);

  std::string name();

  std::shared_ptr<moodycamel::ConcurrentQueue<T>>
  add_publisher(const Node &publisher);

  moodycamel::ReaderWriterQueue<T> &add_subscriber(const Node &subscriber);

  std::thread loop();

  std::string debug();
};

// per https://www.learncpp.com/cpp-tutorial/template-classes/
// you should put all your template class code in the header
// otherwise, the linker will not be able to link properly
template <typename T> Topic<T>::Topic(std::string topic_name) {
  topic_name_ = topic_name;
  publisher_queue_ = std::make_shared<moodycamel::ConcurrentQueue<T>>();
}

template <typename T> std::string Topic<T>::name() { return topic_name_; }

template <typename T>
std::shared_ptr<moodycamel::ConcurrentQueue<T>>
Topic<T>::add_publisher(const Node &publisher) {
  publisher_names_.push_back(publisher.name());
  return publisher_queue_;
}

template <typename T>
moodycamel::ReaderWriterQueue<T> &
Topic<T>::add_subscriber(const Node &subscriber) {
  subscriber_names_.push_back(subscriber.name());
  moodycamel::ReaderWriterQueue<T> new_subscriber_queue;
  subscriber_queues_.push_back(new_subscriber_queue);
  return new_subscriber_queue;
}

template <typename T> std::thread Topic<T>::loop() {
  // TODO: ensure this can only be called once per thread, make private + put in
  // the constructor
  std::thread loop_thread([this]() {
    while (true) {
      T message;
      bool new_message_arrived = publisher_queue_->try_dequeue(message);
      if (new_message_arrived) {
        printf("new_message_arrived\n");
        if (debug_) {
          std::cout << std::to_string(message) << std::endl;
        }

        for (auto &subscriber_queue : subscriber_queues_) {
          subscriber_queue.enqueue(message);
        }
      }

      std::this_thread::sleep_for(
          std::chrono::milliseconds((int)(1000.0 / TOPIC_TICK_RATE)));
    }
  });

  return loop_thread;
}

template <typename T> std::string Topic<T>::debug() {
  debug_ = true;
  // TODO: print current status of subs/pubs
  return topic_name_;
}
// TODO: un-debug?
