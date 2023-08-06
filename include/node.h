#pragma once

#include <string>
#include <thread>

class Node {
public:
  virtual const std::string &name() const = 0;
  virtual const int &tick_rate() const = 0;

  virtual void setup() = 0;
  virtual std::thread loop() = 0;

  virtual ~Node() = default;
};
