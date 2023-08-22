#include <stdio.h>
#include <gtest/gtest.h>
#include <vector>

#include "topic.h"
#include "int_pub_node.h"
#include "int_sub_node.h"

// TODO: rename this test suite to topic&node test

TEST(TopicTest, ConstructorAndGetName) {
  std::string topic_name = "int_topic";
  Topic<int> int_topic = Topic<int>(topic_name);
  ASSERT_EQ(int_topic.name(), topic_name);
}

TEST(TopicTest, Debug) {
  std::string topic_name = "int_topic";
  Topic<int> int_topic = Topic<int>(topic_name);
  std::string debug_status = int_topic.debug();
  ASSERT_EQ(debug_status, topic_name);
}

TEST(TopicTest, SinglePubSub) {
  std::string topic_name = "int_topic";
  Topic<int> int_topic = Topic<int>(topic_name);
  std::thread topic_thread = int_topic.loop();
  // int_topic.debug();

  const int tick_rate_hz = 10;
  const int num_ints = 10;
  IntPublisherNode int_pub_node =
      IntPublisherNode("int_pub_node", tick_rate_hz, int_topic, num_ints);
  std::thread node_thread = int_pub_node.loop();

  IntSubscriberNode int_sub_node =
      IntSubscriberNode("int_sub_node", 10, int_topic);
  std::thread sub_node_thread = int_sub_node.loop();

  // TODO: this should be in setup(), how to get setup more flexible?
  // call setup() thread, block for X seconds, send CV to break setup, then go
  // to loop() thread?
  int_sub_node.set_expected(int_pub_node.get_planned_send());

  std::thread check_thread([int_sub_node]() {
    while (true) {
      if (int_sub_node.done()) {
        std::cout << "DONE DONE DONE" << std::endl;
      } else {
        // std::cout << "NO" << std::endl;
      }
      std::this_thread::sleep_for(
          std::chrono::milliseconds((int)(1000.0 / 1)));
    }
  });
  check_thread.join();

  // TODO: add sub to prove the pub/sub mechanism works
  // e.g. pub 10 ints, sub to the topic
  // once all 10 received, std::terminate() to kill all threads? or some way to
  // kill the threads before timeout?
  topic_thread.join();
  node_thread.join();
  sub_node_thread.join();
  // exit(0);
}
