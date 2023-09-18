#pragma once

#include <string>
#include <thread>

/*
 * Pure virtual class (interface) which defines how a Topic expects Nodes to
 * behave. See <TODO filename here> for example of a Node which can publish and
 * subscribe to any number of Topics.
 */
class Node {
public:
  /*
   * @return the name of this node.
   */
  virtual const std::string &name() const = 0;

  /*
   * Main thread is guaranteed to call setup() on all nodes, then wait for
   * ready(), before calling loop() on ANY node.
   *
   * This method should spin up a thread and set the return value of ready() to
   * be true when that thread terminates.
   */
  virtual void setup() = 0;

  /*
   * @return whether the node is done setting up via setup().
   */
  // virtual bool ready() = 0;

  /*
   * Main thread is guaranteed to call loop() only after all other nodes have
   * finished setting up via setup().
   *
   * This method should spin up a thread which detaches immediately, but can be
   * killed by the kill() method below.
   */
  virtual void loop() = 0;

  /*
   * When kill() is called, the thread spun up by loop() should stop executing.
   */
  virtual void kill() = 0;

  /*
   * Necessary for pure virtual classes to prevent unexpected destructor behavior.
   */
  virtual ~Node() = default;
};
