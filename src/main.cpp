#include <exception>
#include <iostream>
#include <string>

#include <CLI/App.hpp>
#include <CLI/CLI.hpp>

#include "command_parser.h"
#include "file_sink.h"
#include "stream_sink.h"

int main(int argc, char *argv[]) {
  CLI::App app{"Command block packer", "cmd"};

  std::size_t n{};
  app.add_option("-n", n, "Size of block")->required();

  CLI11_PARSE(app, argc, argv);

  auto file_sink = std::make_shared<sink::FileSink>();
  auto stream_sink = std::make_shared<sink::StreamSink>(std::cout);

  parser::CommandParser parser{n};
  parser.register_sink(file_sink);
  parser.register_sink(stream_sink);

  try {
    std::string command_line;
    while (std::getline(std::cin, command_line)) {
      parser.feed(command_line);
    }
    parser.end();
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}