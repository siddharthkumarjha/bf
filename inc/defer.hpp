#pragma once
#include "functional"

class defer_impl
{
    std::function<void(void)> m_Func;

public:
    defer_impl() = delete;
    defer_impl(auto &&func) : m_Func(std::move(func)) {}
    ~defer_impl()
    {
        if (m_Func)
            m_Func();
    }
};

#define CAT2(x, y) x##y
#define CAT(x, y) CAT2(x, y)
#define DEFER_OBJ_NAME CAT(defer_call_, __LINE__)
#define defer defer_impl DEFER_OBJ_NAME = [&]() -> void
