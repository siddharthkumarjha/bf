#pragma once
#include "sstream"
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

template <typename... Args> inline void Panic(const Args &...log_args)
{
    std::ostringstream oss;
    oss << "PANIC: ";
    (oss << ... << log_args);
    throw std::runtime_error(oss.str());
}

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

inline std::unordered_map<INS, const char *> g_INS_to_str = {
    {INS::MOV_DP_RIGHT, "MOV_DP_RIGHT"},
    {INS::MOV_DP_LEFT, "MOV_DP_LEFT"},
    {INS::INC_BYTE, "INC_BYTE"},
    {INS::DEC_BYTE, "DEC_BYTE"},
    {INS::OUT_BYTE, "OUT_BYTE"},
    {INS::ACCEPT_INPUT, "ACCEPT_INPUT"},
    {INS::COND_JMP_START, "COND_JMP_START"},
    {INS::COND_JMP_END, "COND_JMP_END"}};

inline constexpr std::string BF_EXTENSIONS[] = {".b", ".bf"};

namespace detail
{
/**
 * @return val
 * @bool - debug_flag
 * @bool - is_bf_file_flag
 */
std::pair<bool, bool> parse_cmd_line(int &argc, char **&argv);
std::vector<char> parse_bf_tokens(char **argv);
void print_bf_tokens(std::vector<char> &tokens);
} // namespace detail
} // namespace bf
