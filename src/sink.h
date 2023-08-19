#ifndef SINK
#define SINK

#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace sink {

/**
 * @brief Sink interface for saving bulk of commands
 *
 */
class Sink {
 public:
  virtual ~Sink() = default;

  /**
   * @brief Save bulk of commands
   *
   * @param commands            bulk
   * @param first_command_ts    first commands receive timestamp
   */
  virtual void save(const std::vector<std::string> &commands,
                    std::chrono::system_clock::time_point first_command_ts) = 0;
};

}  // namespace sink

#endif
