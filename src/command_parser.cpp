#include "command_parser.h"

#include <chrono>

#include "sink.h"

namespace parser {

void CommandParser::register_sink(std::weak_ptr<sink::Sink> sink) {
  _sinks.push_back(std::move(sink));
}

void CommandParser::feed(std::string command) {
  if (command == BLOCK_START) {
    enter_block();
  } else if (command == BLOCK_END) {
    exit_block();

    if (is_not_nested()) {
      save_to_sinks({});
    }
  } else {
    add_command(std::move(command));

    if (is_full_block() && is_not_nested()) {
      save_to_sinks({});
    }
  }
}

void CommandParser::enter_block() {
  if (is_not_nested()) {
    save_to_sinks({});
  }

  _nested++;
}

void CommandParser::exit_block() {
  if (is_not_nested()) {
    throw NestedError{};
  }
  _nested--;
}

void CommandParser::end() {
  if (is_not_nested()) {
    save_to_sinks({});
  }
}

void CommandParser::save_to_sinks(std::chrono::system_clock::time_point ts) {
  if (!has_commands()) {
    return;
  }

  for (auto &sink : _sinks) {
    if (auto ptr = sink.lock(); ptr != nullptr) {
      ptr->save(_current_commands, ts);
    }
  }
  _current_commands.clear();
}

}  // namespace parser