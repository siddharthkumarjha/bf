#pragma once
#include "sstream"
#include "streamable_array.hpp"
#include <format>
#include <source_location>
#include <stdexcept>

template <streamable... Args> inline void panic_impl(const Args &...log_args)
{
    std::ostringstream oss;
    oss << "PANIC: ";
    (oss << ... << log_args);
    oss << '\n';
    throw std::runtime_error(std::move(oss).str());
}

template <streamable... Ts> struct Panic
{
    constexpr std::string_view strrchr(const std::string_view str,
                                       const char match)
    {
        auto begin_itr        = str.begin();
        const auto end_itr    = str.end();
        auto last_matched_itr = begin_itr;
        for (; begin_itr != end_itr; ++begin_itr)
        {
            if (*begin_itr == match)
                last_matched_itr = begin_itr + 1;
        }

        return std::string_view{last_matched_itr, end_itr};
    }

    constexpr std::string_view trim_function_name(const std::string_view func)
    {
        size_t end_pos   = func.find('(');
        size_t start_pos = func.find(' ');
        start_pos = (start_pos != std::string_view::npos) ? (start_pos + 1) : 0;
        end_pos   = (end_pos != std::string_view::npos) ? end_pos : func.size();
        return func.substr(start_pos, (end_pos - start_pos));
    }

    constexpr Panic(Ts &&...args, std::source_location const location =
                                      std::source_location::current())
    {
        std::string prefix = std::format(
            "[{}:{}:{}] ", strrchr(location.file_name(), '/'), location.line(),
            trim_function_name(location.function_name()));
        panic_impl(prefix, std::forward<Ts>(args)...);
    }
};

template <typename... Ts> Panic(Ts &&...) -> Panic<Ts...>;
