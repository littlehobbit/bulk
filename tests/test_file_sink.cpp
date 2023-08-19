#include <chrono>
#include <fstream>
#include <sstream>

#include <gtest/gtest.h>

#include "file_sink.h"

class FileSinkTest : public ::testing::Test {
 public:
  sink::FileSink file_sink{};

  bool log_file_exist(std::chrono::system_clock::time_point ts) {
    std::ostringstream file_name;
    file_name << "bulk" << ts.time_since_epoch().count() << ".log";
    std::ifstream file{file_name.str()};
    return file.is_open();
  }
};

// NOLINTNEXTLINE
TEST_F(FileSinkTest, OnSave_WriteToFile) {
  auto now = std::chrono::system_clock::now();
  file_sink.save({"cmd1", "cmd2"}, now);
  ASSERT_TRUE(log_file_exist(now));
}