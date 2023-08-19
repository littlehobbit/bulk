#ifndef UTILS
#define UTILS

#include <algorithm>
#include <iterator>
#include <string_view>

namespace utils {

template <typename Stream, typename Range>
void write_with_sep(Stream &stream, const Range &range,
                    std::string_view sep) noexcept {
  std::copy(std::begin(range), std::prev(std::end(range)),
            std::ostream_iterator<std::string>(stream, sep.data()));
  stream << *std::prev(std::end(range)) << std::endl;
}

}  // namespace utils

#endif
