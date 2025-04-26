#pragma once
#include "cstring"
#include "panic.hpp"
#include "streamable_array.hpp"
#include <bitset>
#include <unordered_map>
#include <vector>

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

    inline constexpr auto BF_EXTENSIONS =
        make_array<std::string_view>(".b", ".bf");

    inline namespace detail
    {
        inline constexpr auto EXPECTED_FLAGS =
            make_array<std::string_view>("--debug", "--interpret", "--compile");

        enum FLAG : int
        {
            DEBUG_FLAG = 0,
            INTERPRET_FLAG,
            COMPILE_FLAG,
            FLAG_SIZE
        };

        struct Cli_Options
        {
            std::bitset<FLAG_SIZE> flags = {0};
            std::string_view file_name;

            std::bitset<FLAG_SIZE>::reference operator[](FLAG idx)
            {
                return flags[idx];
            }
            bool operator[](FLAG idx) const { return flags[idx]; }
        };

        Cli_Options parse_cmd_line(int &argc, char **&argv);
    } // namespace detail

    inline namespace preproc
    {
        std::vector<ops> parse_tokens(const std::string_view);
        void print_tokens(std::vector<ops> &tokens);
    } // namespace preproc

    inline namespace interpretor
    {
        void interpret(std::vector<bf::ops> const &tokens, bool debug_flag);
    }
} // namespace bf
