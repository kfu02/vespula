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

/*
 * IntPublisherNode sends 10 random ints to IntSubscriberNode via int_topic. 
 *
 * Tick rate is set to 10 hz, so this should take around 1 second.
 */
TEST(TopicTest, SinglePubSub) {
  std::string topic_name = "int_topic";
  Topic<int> int_topic = Topic<int>(topic_name);
  // int_topic.debug();

  const int tick_rate_hz = 10;
  const int num_ints = 10;
  IntPublisherNode int_pub_node =
      IntPublisherNode("int_pub_node", tick_rate_hz, int_topic, num_ints);

  IntSubscriberNode int_sub_node =
      IntSubscriberNode("int_sub_node", 10, int_topic);

  // TODO: how to get setup more flexible?
  // call setup() thread, block for X seconds, send CV to break setup, then go
  // to loop() thread?

  int_topic.loop();
  int_pub_node.loop();
  int_sub_node.loop();

  while (int_sub_node.get_received().size() < int_pub_node.get_planned_send().size()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  int_topic.kill();
  int_pub_node.kill();
  int_sub_node.kill();

  // notice the data still persists even after the node is "killed"
  // TODO: should I make a way to restart node? would be as simple as changing
  // "break" to "continue" in the "killed" if statement
  std::vector<int> &received = int_sub_node.get_received();
  std::vector<int> &expected = int_pub_node.get_planned_send();

  printf("Pub sent: ");
  for (const int &i : expected) {
    printf("%d, ", i);
  }
  printf("\nSub received: ");
  for (const int &i : received) {
    printf("%d, ", i);
  }
  printf("\n");

  ASSERT_EQ(received, expected);
}

TEST(TopicTest, KillNodeLoop) {
  std::string topic_name = "int_topic";
  Topic<int> int_topic = Topic<int>(topic_name);
  // int_topic.debug();

  int tick_rate_hz = 10;
  IntPublisherNode int_pub_node =
      IntPublisherNode("int_pub_node", tick_rate_hz, int_topic, 10);

  IntSubscriberNode int_sub_node =
      IntSubscriberNode("int_sub_node", 10, int_topic);

  int_topic.loop();
  int_pub_node.loop();
  int_sub_node.loop();

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  int_topic.kill();
  int_pub_node.kill();
  int_sub_node.kill();

  // by killing in < 100ms, sub should have received nothing
  ASSERT_EQ(int_sub_node.get_received().size(), 0);
}

// TODO: add test for MultiPubMultiSub (requires writing a more general Node which takes vectors of pub/sub nodes)
