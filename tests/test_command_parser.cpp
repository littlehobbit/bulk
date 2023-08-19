#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "command_parser.h"
#include "gmock/gmock.h"
#include "sink.h"

class MockSink : public sink::Sink {
 public:
  MOCK_METHOD(void, save,
              (const std::vector<std::string> &commands,
               std::chrono::system_clock::time_point first_command_ts),
              (override));
};

using namespace ::testing;

class CommandParserTest : public ::testing::Test {
 public:
  void SetUp() override { parser.register_sink(mock_sink); }

  static constexpr auto BLOCK_SIZE = 3;
  parser::CommandParser parser{BLOCK_SIZE};
  std::shared_ptr<MockSink> mock_sink = std::make_shared<MockSink>();
};

// NOLINTNEXTLINE
TEST_F(CommandParserTest, OnParseBlockSize_CallSave_AndThenSaveLastOnEnd) {
  EXPECT_CALL(*mock_sink,  //
              save(ElementsAreArray({"cmd1", "cmd2", "cmd3"}), _))
      .WillOnce(DoDefault());

  parser.feed("cmd1");
  parser.feed("cmd2");
  parser.feed("cmd3");

  EXPECT_CALL(*mock_sink,  //
              save(ElementsAreArray({"cmd4", "cmd5"}), _))
      .WillOnce(DoDefault());

  parser.feed("cmd4");
  parser.feed("cmd5");
  parser.end();
}

// NOLINTNEXTLINE
TEST_F(CommandParserTest, OnDynblockBegin_SavePrevCommands) {
  EXPECT_CALL(*mock_sink,  //
              save(ElementsAreArray({"cmd1", "cmd2"}), _))
      .WillOnce(DoDefault());

  parser.feed("cmd1");
  parser.feed("cmd2");
  parser.feed("{");
  parser.feed("}");
}

// NOLINTNEXTLINE
TEST_F(CommandParserTest, OnNestedDynblocksCloses_SaveAllCommands) {
  EXPECT_CALL(*mock_sink,  //
              save(ElementsAreArray({"cmd1", "cmd2", "cmd3", "cmd4", "cmd5",
                                     "cmd6", "cmd7", "cmd8"}),
                   _))
      .WillOnce(DoDefault());

  parser.feed("{");
  parser.feed("cmd1");
  parser.feed("cmd2");
  parser.feed("{");
  parser.feed("cmd3");
  parser.feed("cmd4");
  parser.feed("{");
  parser.feed("cmd5");
  parser.feed("cmd6");
  parser.feed("}");
  parser.feed("}");
  parser.feed("cmd7");
  parser.feed("cmd8");
  parser.feed("}");
}

// NOLINTNEXTLINE
TEST_F(CommandParserTest, OnEnd_WhenDynblockOpen_DiscardsNestedCommands) {
  EXPECT_CALL(*mock_sink, save).Times(0);

  parser.feed("{");
  parser.feed("cmd1");
  parser.feed("cmd2");
  parser.feed("{");
  parser.feed("cmd3");
  parser.feed("cmd4");
  parser.feed("{");
  parser.end();
}

// NOLINTNEXTLINE
TEST_F(CommandParserTest, OnCloseNonOpenedBlock_ThrowError) {
  EXPECT_THROW(parser.feed("}"), parser::NestedError);
}
