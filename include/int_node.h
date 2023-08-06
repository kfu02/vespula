#pragma once

#include <string>

#include "node.h"
#include "topic.h"
#include "concurrentqueue.h"

/*
 * Class used for testing which simply publishes an int every <tick-rate> hz.
 */
class IntPublisherNode : public Node {
private:
  std::string name_;
  int tick_rate_;

  std::weak_ptr<moodycamel::ConcurrentQueue<int>> pub_queue_;

public:
  IntPublisherNode(std::string name, int tick_rate, Topic<int> &pub_topic);
  ~IntPublisherNode();

  const std::string &name() const override;
  const int &tick_rate() const override;

  void setup() override;
  std::thread loop() override;
};
