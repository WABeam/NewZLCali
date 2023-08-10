#pragma once

#include <stdexcept>
#include <string>

namespace core {
class Exception : public std::runtime_error {
public:
    Exception(const std::string& whatArg) : std::runtime_error(whatArg) {}
    Exception(const char* whatArg) : std::runtime_error(whatArg) {}
};
} // namespace core
