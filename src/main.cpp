#include "bf.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>

constexpr auto max(auto x, auto y) { return (x < y) ? y : x; }
constexpr auto min(auto x, auto y) { return (x < y) ? x : y; }

int main(int argc, char *argv[])
{
    try
    {
        auto [debug_flag, is_bf_file_flag] =
            bf::detail::parse_cmd_line(argc, argv);
        if (not is_bf_file_flag)
        {
            Panic("bf file not found");
        }

        std::vector<char> tokens = bf::detail::parse_bf_tokens(argv);
        if (debug_flag)
            bf::detail::print_bf_tokens(tokens);

        auto ip             = tokens.begin();
        const auto ip_end   = tokens.end();
        const auto ip_begin = tokens.begin() - 1;

        int8_t cells[bf::MAX_STACK_SIZE] = {0};
        int8_t *dp                       = cells;
        const int8_t *const dp_begin     = cells - 1;
        const int8_t *const dp_end       = cells + bf::MAX_STACK_SIZE;

        while (ip != ip_end)
        {
            if (dp <= dp_begin)
                Panic("stack underflow dp offset:: ", (dp_begin - dp) - 1);
            else if (dp >= dp_end)
                Panic("stack overflow dp offset:: ", (dp - dp_end) + 1);

            if (debug_flag)
                std::printf("[%ld] %d %s\n", (dp - (dp_begin + 1)), *dp,
                            bf::g_INS_to_str.at(static_cast<bf::INS>(*ip)));

            switch (static_cast<bf::INS>(*ip))
            {
            case bf::MOV_DP_RIGHT:
            {
                ++dp;
                ++ip;
            }
            break;
            case bf::MOV_DP_LEFT:
            {
                --dp;
                ++ip;
            }
            break;
            case bf::INC_BYTE:
            {
                *dp += 1;
                ++ip;
            }
            break;
            case bf::DEC_BYTE:
            {
                *dp -= 1;
                ++ip;
            }
            break;
            case bf::OUT_BYTE:
            {
                printf("%c", *dp);
                ++ip;
            }
            break;
            case bf::ACCEPT_INPUT:
            {
                printf("\nWaiting for 1byte input:: ");
                *dp = getchar();
                ++ip;
            }
            break;
            case bf::COND_JMP_START:
            {
                if (*dp == 0) // jmp to ins after end, exit loop
                {
                    while (ip != ip_end && *ip != bf::COND_JMP_END)
                        ++ip;
                    if (ip == ip_end)
                        Panic("Matching ] not found");
                }
                // go to next ins
                ++ip;
            }
            break;
            case bf::COND_JMP_END:
            {
                if (*dp != 0) // jmp to ins after start
                {
                    while (ip != ip_begin && *ip != bf::COND_JMP_START)
                        --ip;
                    if (ip == ip_begin)
                        Panic("Matching [ not found");
                }
                // goto next ins / exit loop
                ++ip;
            }
            break;
            default:
            {
                Panic("Unexpected input token found");
            }
            break;
            }
        }
    }
    catch (std::exception const &excuse)
    {
        std::cerr << excuse.what() << std::endl;
        std::cerr << "exiting..." << std::endl;

        return 1;
    }

    return 0;
}
