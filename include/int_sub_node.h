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

  // TODO: thread safe these vectors? if read-only after set it's not wrong, but...?
  std::vector<int> received_;
  std::vector<int> expected_;

  bool done_;

public:
  IntSubscriberNode(std::string name, int tick_rate, Topic<int> &sub_topic);
  ~IntSubscriberNode();

  const std::string &name() const override;
  const int &tick_rate() const override;

  void setup() override;
  std::thread loop() override;

  std::vector<int> &get_received();

  void set_expected(std::vector<int> expected);
  bool done() const;
};
