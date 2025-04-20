#include "bf.hpp"

void bf::interpretor::interpret(std::vector<bf::ops> const &tokens,
                                bool debug_flag)
{

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"

    auto ip             = tokens.cbegin();
    const auto ip_end   = tokens.cend();
    const auto ip_begin = tokens.cbegin() - 1;

    int8_t cells[bf::MAX_STACK_SIZE] = {0};
    int8_t *dp                       = cells;
    const int8_t *const dp_begin     = cells - 1;
    const int8_t *const dp_end       = cells + bf::MAX_STACK_SIZE;

#pragma GCC diagnostic pop

    if (debug_flag)
    {
        std::printf("\n-------------------------------\n");
        std::printf("off\t*dp\tINS\n");
        std::printf("-------------------------------\n");
    }

    while (ip != ip_end)
    {
        if (dp <= dp_begin) [[unlikely]]
            Panic("stack underflow dp offset:: ", dp - (dp_begin + 1));
        else if (dp >= dp_end) [[unlikely]]
            Panic("stack overflow dp offset:: ", dp - (dp_end - 1));

        if (ip <= ip_begin) [[unlikely]]
            Panic("ip went out of range");
        else if (ip >= ip_end) [[unlikely]]
            Panic("ip went out of range");

        if (ip->operand <= 0) [[unlikely]]
            Panic("Some token parsing error happened");

        if (debug_flag)
            std::printf("[%ld]\t%d\t%s\n", (dp - (dp_begin + 1)), *dp,
                        bf::g_INS_to_str.at(ip->op));

        // todo; fix loops; make an AST
        switch (ip->op)
        {
        case bf::MOV_DP_RIGHT:
        {
            dp += ip->operand;
            ++ip;
        }
        break;
        case bf::MOV_DP_LEFT:
        {
            dp -= ip->operand;
            ++ip;
        }
        break;
        case bf::INC_BYTE:
        {
            *dp += ip->operand;
            ++ip;
        }
        break;
        case bf::DEC_BYTE:
        {
            *dp -= ip->operand;
            ++ip;
        }
        break;
        case bf::OUT_BYTE:
        {
            for (size_t i = 0; i < ip->operand; ++i)
                printf("%c", *dp);
            ++ip;
        }
        break;
        case bf::ACCEPT_INPUT:
        {
            printf("\ninput> ");
            for (size_t i = 0; i < ip->operand; ++i)
                *dp = getchar();
            ++ip;
        }
        break;
        case bf::COND_JMP_START:
        {
            if (*dp == 0) // jmp to ins after end, exit loop
            {
                ip = (ip_begin + 1) + ip->operand;
            }
            // go to next ins
            ++ip;
        }
        break;
        case bf::COND_JMP_END:
        {
            if (*dp != 0) // jmp to ins after start
            {
                ip = (ip_begin + 1) + ip->operand;
            }
            // goto next ins / exit loop
            ++ip;
        }
        break;
        [[unlikely]] default:
        {
            Panic("Unexpected input token found");
        }
        break;
        }
    }
}
