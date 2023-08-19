#ifndef SINK
#define SINK

#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace sink {

class Sink {
 public:
  virtual ~Sink() = default;

  virtual void save(const std::vector<std::string> &commands,
                    std::chrono::system_clock::time_point first_command_ts) = 0;
};

}  // namespace sink

#endif
