#include "int_pub_node.h"

IntPublisherNode::IntPublisherNode(std::string name, int tick_rate,
                                   Topic<int> &pub_topic, int num_ints) {
  // TODO: initializer list here
  name_ = name;
  tick_rate_ = tick_rate;
  pub_queue_ = pub_topic.add_publisher(*this);

  planned_send_ = generate_random_nums(num_ints);
}

IntPublisherNode::~IntPublisherNode() {
  // free(pub_queue_);
}

std::vector<int> IntPublisherNode::generate_random_nums(int size) {
  std::srand(unsigned(std::time(nullptr)));
  std::vector<int> v(size);
  std::generate(v.begin(), v.end(), std::rand);
  return v;
}

const std::string &IntPublisherNode::name() const { return name_; }

const int &IntPublisherNode::tick_rate() const { return tick_rate_; }

void IntPublisherNode::setup() {
  // TODO: RNG some numbers, make it possible to access from the outside
}

std::thread IntPublisherNode::loop() {
  // TODO: ensure this can only be called once per thread, make private + put in
  // the constructor
  std::thread loop_thread([this]() {
    for (const int &i : planned_send_) {
      std::shared_ptr<moodycamel::ConcurrentQueue<int>> shared_pub_queue =
          pub_queue_.lock();

      if (!shared_pub_queue) {
        // queue no longer valid
        printf("invalid queue\n");
        break;
      }

      std::cout << name_ << " published: " << i << std::endl;
      shared_pub_queue->enqueue(i);
      // int out = 0;
      // shared_pub_queue->try_dequeue(out);
      // printf("out: %d\n", out);

      std::this_thread::sleep_for(
          std::chrono::milliseconds((int)(1000.0 / tick_rate_)));
    }
  });
  return loop_thread;
}

std::vector<int> &IntPublisherNode::get_planned_send() {
  return planned_send_;
}
