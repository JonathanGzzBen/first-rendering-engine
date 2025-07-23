#ifndef ERROR_H
#define ERROR_H

#include <string>

namespace frengine {
using Error = struct Error {
  std::string message;
};

}  // namespace frengine

#endif  // ERROR_H
