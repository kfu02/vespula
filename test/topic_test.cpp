#include <stdio.h>
#include <gtest/gtest.h>

#include "topic.h"
#include "int_node.h"

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

TEST(TopicTest, AddPublisher) {
  std::string topic_name = "int_topic";
  Topic<int> int_topic = Topic<int>(topic_name);
  std::thread topic_thread = int_topic.loop();
  int_topic.debug();

  std::string node_name = "int_node";
  int tick_rate_hz = 1;
  IntPublisherNode int_node =
      IntPublisherNode(node_name, tick_rate_hz, int_topic);
  std::thread node_thread = int_node.loop();

  // TODO: add sub to prove the pub/sub mechanism works
  // e.g. pub 10 ints, sub to the topic
  // once all 10 received, std::terminate() to kill all threads? or some way to
  // kill the threads before timeout?
  topic_thread.join();
  node_thread.join();
  // exit(0);
}
