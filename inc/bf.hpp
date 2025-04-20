#pragma once
#include "cstring"
#include "sstream"
#include <array>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

template <std::size_t Nm>
inline std::ostream &operator<<(std::ostream &os,
                                const std::array<std::string, Nm> &arr)
{
    const char *sep = "[";
    if constexpr (Nm == 0)
        os << sep;

    for (const auto &arr_elem : arr)
    {
        os << sep << arr_elem;
        sep = ", ";
    }

    os << "]";
    return os;
}

template <typename... Args> inline void panic_impl(const Args &...log_args)
{
    std::ostringstream oss;
    oss << "PANIC: ";
    (oss << ... << log_args);
    oss << '\n';
    throw std::runtime_error(oss.str());
}

#define FILENAME strrchr("/" __FILE__, '/') + 1
#define Panic(...)                                                             \
    panic_impl('[', FILENAME, ':', __LINE__, ':', __FUNCTION__, "] ",          \
               __VA_ARGS__)

namespace bf
{
    inline constexpr const size_t MAX_STACK_SIZE = 30'000;
    enum INS : char
    {
        MOV_DP_RIGHT   = '>',
        MOV_DP_LEFT    = '<',
        INC_BYTE       = '+',
        DEC_BYTE       = '-',
        OUT_BYTE       = '.',
        ACCEPT_INPUT   = ',',
        COND_JMP_START = '[', // if byte at DP is 0, goto end
        COND_JMP_END   = ']'  // if byte at DP is !0, goto begin
    };

    struct ops
    {
        INS op;
        size_t operand;
    };

    inline std::unordered_map<INS, const char *> g_INS_to_str = {
        {INS::MOV_DP_RIGHT, "MOV_DP_RIGHT"},
        {INS::MOV_DP_LEFT, "MOV_DP_LEFT"},
        {INS::INC_BYTE, "INC_BYTE"},
        {INS::DEC_BYTE, "DEC_BYTE"},
        {INS::OUT_BYTE, "OUT_BYTE"},
        {INS::ACCEPT_INPUT, "ACCEPT_INPUT"},
        {INS::COND_JMP_START, "COND_JMP_START"},
        {INS::COND_JMP_END, "COND_JMP_END"}};

    inline constexpr std::array<std::string, 2> BF_EXTENSIONS = {".b", ".bf"};

    inline namespace detail
    {
        /**
         * @return val
         * @bool - debug_flag
         * @string - bf_file_name
         */
        std::pair<bool, std::string> parse_cmd_line(int &argc, char **&argv);
    } // namespace detail

    inline namespace preproc
    {
        std::vector<ops> parse_bf_tokens(const std::string &);
        void print_bf_tokens(std::vector<ops> &tokens);
    } // namespace preproc

    inline namespace interpretor
    {
        void interpret(std::vector<bf::ops> const &tokens, bool debug_flag);
    }
} // namespace bf
