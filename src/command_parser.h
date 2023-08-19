#ifndef COMMAND_PARSER
#define COMMAND_PARSER

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace sink {
class Sink;
}

namespace parser {

class ParserError : public std::runtime_error {
 public:
  explicit ParserError(const std::string &what) : std::runtime_error{what} {}
};

class NestedError : public ParserError {
 public:
  NestedError() : ParserError{"invalid curly braces sequence"} {}
};

class CommandParser {
 public:
  explicit CommandParser(std::size_t block_size) : _block_size(block_size) {}

  void register_sink(std::weak_ptr<sink::Sink> sink);

  void feed(std::string command);

  void end();

 private:
  void save_to_sinks();

  void enter_block();
  void exit_block();

  bool is_not_nested() const noexcept { return _nested == 0; }

  bool is_full_block() const noexcept {
    return _current_commands.size() == _block_size;
  }

  void add_command(std::string command) {
    if (!has_commands()) {
      _first_commands_ts = std::chrono::system_clock::now();
    }

    _current_commands.emplace_back(std::move(command));
  }

  bool has_commands() const noexcept { return !_current_commands.empty(); }

  std::size_t _block_size;
  std::vector<std::weak_ptr<sink::Sink>> _sinks;
  std::vector<std::string> _current_commands;
  std::size_t _nested{0};
  std::chrono::system_clock::time_point _first_commands_ts{};

  static constexpr auto BLOCK_START = "{";
  static constexpr auto BLOCK_END = "}";
};

}  // namespace parser

#endif
