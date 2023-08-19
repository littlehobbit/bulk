#ifndef STREAM_SINK
#define STREAM_SINK

#include <ostream>

#include "sink.h"
#include "utils.h"

namespace sink {

class StreamSink : public Sink {
 public:
  explicit StreamSink(std::ostream &stream) : _stream{stream} {}

  void save(const std::vector<std::string> &commands,
            std::chrono::system_clock::time_point first_command_ts) override {
    _stream << "bulk: ";
    utils::write_with_sep(_stream, commands, ", ");
  }

 private:
  std::ostream &_stream;
};

}  // namespace sink

#endif
