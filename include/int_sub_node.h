#pragma once

#include <string>
#include <vector>

#include "node.h"
#include "topic.h"
#include "readerwriterqueue.h"

/*
 * TODO docs
 */
class IntSubscriberNode : public Node {
private:
  std::string name_;
  int tick_rate_;

  std::weak_ptr<moodycamel::ReaderWriterQueue<int>> sub_queue_;

  std::vector<int> received_;

  bool done_;
  std::atomic_bool kill_signal_{false};

public:
  IntSubscriberNode(std::string name, int tick_rate, Topic<int> &sub_topic);
  ~IntSubscriberNode();

  const std::string &name() const override;
  const int &tick_rate() const;

  void setup() override;
  void loop() override;
  void kill() override;

  // TODO: maybe is better design to read-only from Node methods?
  std::vector<int> &get_received();
};
