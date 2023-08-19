#ifndef FILE_SINK
#define FILE_SINK

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "sink.h"
#include "utils.h"

namespace sink {

class FileSink : public Sink {
 public:
  void save(const std::vector<std::string> &commands,
            std::chrono::system_clock::time_point first_command_ts) override {
    const auto file_name = create_filename(first_command_ts);
    std::ofstream file{file_name};

    utils::write_with_sep(file, commands, ", ");
  }

 private:
  auto create_filename(std::chrono::system_clock::time_point ts) const
      -> std::string {
    std::ostringstream stream;
    stream << "bulk" << ts.time_since_epoch().count() << ".log";
    return stream.str();
  }
};

}  // namespace sink

#endif
