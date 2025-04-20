#include "bf.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
    try
    {
        auto [debug_flag, bf_file_name] = bf::parse_cmd_line(argc, argv);

        std::vector<char> tokens = bf::parse_bf_tokens(bf_file_name);
        if (debug_flag)
            bf::print_bf_tokens(tokens);

        auto ip             = tokens.begin();
        const auto ip_end   = tokens.end();
        const auto ip_begin = tokens.begin() - 1;

        int8_t cells[bf::MAX_STACK_SIZE] = {0};
        int8_t *dp                       = cells;
        const int8_t *const dp_begin     = cells - 1;
        const int8_t *const dp_end       = cells + bf::MAX_STACK_SIZE;

        if (debug_flag)
        {
            std::printf("\n-------------------------------\n");
            std::printf("off\t*dp\tINS\n");
            std::printf("-------------------------------\n");
        }

        while (ip != ip_end)
        {
            if (dp <= dp_begin)
                Panic("stack underflow dp offset:: ", dp - (dp_begin + 1));
            else if (dp >= dp_end)
                Panic("stack overflow dp offset:: ", (dp_end - 1) - dp);

            if (debug_flag)
                std::printf("[%ld]\t%d\t%s\n", (dp - (dp_begin + 1)), *dp,
                            bf::g_INS_to_str.at(static_cast<bf::INS>(*ip)));

            // todo; fix loops; make an AST
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
