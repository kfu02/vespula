#pragma once

#include <string>
#include <thread>

class Node {
public:
  virtual const std::string &name() const = 0;

  // TODO: add docstrings
  virtual void setup() = 0;
  // TODO: add end_setup() method
  virtual void loop() = 0;
  virtual void kill() = 0;

  virtual ~Node() = default;
};
