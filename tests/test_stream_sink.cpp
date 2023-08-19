#include <sstream>

#include <gtest/gtest.h>

#include "stream_sink.h"

class StreamSinkTest : public ::testing::Test {
 public:
  std::ostringstream stream;
  sink::StreamSink sink{stream};
};

// NOLINTNEXTLINE
TEST_F(StreamSinkTest, SaveOneCommand) {
  sink.save({"cmd1"}, {});
  ASSERT_EQ(stream.str(), "bulk: cmd1\n");
}

// NOLINTNEXTLINE
TEST_F(StreamSinkTest, SaveMultipleCommand) {
  sink.save({"cmd1", "cmd2", "cmd3"}, {});
  ASSERT_EQ(stream.str(), "bulk: cmd1, cmd2, cmd3\n");
}