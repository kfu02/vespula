#include <stdio.h>
#include <gtest/gtest.h>

TEST(TopicTest, ShouldPass) {
  ASSERT_EQ(0, 0);
}

TEST(TopicTest, ShouldFail) {
  ASSERT_EQ(0, 1);
}
