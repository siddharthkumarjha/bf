#pragma once
#include <vector>
#include <string>

enum INS : char
{
    MOV_IP_RIGHT   = '>',
    MOV_IP_LEFT    = '<',
    INC_BYTE       = '+',
    DEC_BYTE       = '-',
    OUT_BYTE       = '.',
    ACCEPT_INPUT   = ',',
    COND_JMP_START = '[', // if byte at DP is 0, goto end
    COND_JMP_END   = ']'  // if byte at DP is !0, goto begin
};

inline constexpr std::string BF_EXTENSIONS[] = {
    ".bf"
};

namespace bf::detail
{
bool is_bf_file(int &argc, char **&argv);
std::vector<char> parse_bf_tokens(char **argv);
}
