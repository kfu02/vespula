#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "concurrentqueue.h"
#include "readerwriterqueue.h"

#include "node.h"

template <typename T>

#define TOPIC_TICK_RATE 1000

/*
 * Handles communication between nodes with lock-free queues (courtesy of moodycamel).
 *
 * TODO explanation ehre
 */
class Topic {
private:
  // shared_ptr only to allow Nodes to receive weak_ptrs from this class, Topic
  // still owns the publisher queue
  std::shared_ptr<moodycamel::ConcurrentQueue<T>> publisher_queue_;

  std::vector<std::string> publisher_names_;

  std::vector<std::shared_ptr<moodycamel::ConcurrentQueue<T>>> subscriber_queues_;
  std::vector<std::string> subscriber_names_;

  std::string topic_name_;
  bool debug_;
  std::atomic_bool kill_signal_{false};

public:
  Topic(std::string topic_name);

  std::string name();

  std::shared_ptr<moodycamel::ConcurrentQueue<T>>
  add_publisher(const Node &publisher);

  std::shared_ptr<moodycamel::ConcurrentQueue<T>>
  add_subscriber(const Node &subscriber);

  void loop();
  void kill();

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
std::shared_ptr<moodycamel::ConcurrentQueue<T>>
Topic<T>::add_subscriber(const Node &subscriber) {
  subscriber_names_.push_back(subscriber.name());
  auto new_subscriber_queue =
      std::make_shared<moodycamel::ConcurrentQueue<T>>();
  subscriber_queues_.push_back(new_subscriber_queue);
  return new_subscriber_queue;
}

template <typename T> void Topic<T>::loop() {
  std::thread loop_thread([this]() {
    while (true) {
      // Mark the time when the loop starts
      auto loopStartTime = std::chrono::high_resolution_clock::now();

      if (kill_signal_) {
        return;
      }

      T message;
      bool new_message_arrived = publisher_queue_->try_dequeue(message);
      if (new_message_arrived) {
        if (debug_) {
          // TODO: clean debug prints?
          printf("new_message_arrived\n");
          std::cout << std::to_string(message) << std::endl;
        }

        for (auto &subscriber_queue : subscriber_queues_) {
          subscriber_queue->enqueue(message);
        }
      }

      // Calculate how much time has elapsed since the start of the loop
      // auto loopEndTime = std::chrono::high_resolution_clock::now();
      // auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(loopEndTime - loopStartTime);
      //
      // // Check if at least 10 ms have elapsed
      // if (elapsedMilliseconds < std::chrono::milliseconds(1)) {
      //     // Calculate how much time to sleep to reach 1 ms
      //     auto sleepDuration = std::chrono::milliseconds(1) - elapsedMilliseconds;
      //     
      //     // Sleep for the remaining time
      //     std::this_thread::sleep_for(sleepDuration);
      // } else {
      //   printf("WARN: Topic tick rate is too slow to keep up with the incoming messages!");
      // }

      // std::this_thread::sleep_for(
      //     std::chrono::milliseconds((int)(1000.0 / TOPIC_TICK_RATE)));
    }
  });

  loop_thread.detach();
}

template <typename T> std::string Topic<T>::debug() {
  debug_ = true;
  // TODO: print current status of subs/pubs
  return topic_name_;
}
// TODO: un-debug?

template <typename T> void Topic<T>::kill() {
  kill_signal_ = true;
}
