#pragma once
#include "streamable_array.hpp"
#include "sstream"
#include <stdexcept>

template <streamable... Args> inline void panic_impl(const Args &...log_args)
{
    std::ostringstream oss;
    oss << "PANIC: ";
    (oss << ... << log_args);
    oss << '\n';
    throw std::runtime_error(std::move(oss).str());
}

#define FILENAME strrchr("/" __FILE__, '/') + 1
#define Panic(...)                                                             \
    panic_impl('[', FILENAME, ':', __LINE__, ':', __FUNCTION__, "] ",          \
               __VA_ARGS__)

