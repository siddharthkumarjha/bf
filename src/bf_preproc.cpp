#include "bf.hpp"
#include "defer.hpp"
#include "fstream"
#include "iostream"
#include <stack>

bool is_bf_comment(char const c)
{
    switch (c)
    {
    case bf::MOV_DP_RIGHT:
    case bf::MOV_DP_LEFT:
    case bf::INC_BYTE:
    case bf::DEC_BYTE:
    case bf::OUT_BYTE:
    case bf::ACCEPT_INPUT:
    case bf::COND_JMP_START:
    case bf::COND_JMP_END:
        return false;
    default:
        return true;
    }
}

std::vector<bf::ops> bf::preproc::parse_bf_tokens(const std::string &file_name)
{
    if (file_name.empty())
        Panic("No input file found");

    std::ifstream bf_file(file_name);
    defer { bf_file.close(); };

    std::vector<bf::ops> collected_ops;
    std::stack<size_t> track_jmp_ins;

    auto stream_start     = std::istreambuf_iterator<char>(bf_file);
    auto const stream_end = std::istreambuf_iterator<char>();

    for (; stream_start != stream_end; ++stream_start)
    {
        const INS op = static_cast<INS>(*stream_start);
        switch (op)
        {
        case bf::MOV_DP_RIGHT:
        case bf::MOV_DP_LEFT:
        case bf::INC_BYTE:
        case bf::DEC_BYTE:
        case bf::OUT_BYTE:
        case bf::ACCEPT_INPUT:
        {
            if (not collected_ops.empty() and collected_ops.back().op == op)
                ++(collected_ops.back().operand);
            else
                collected_ops.emplace_back(op, 1);
        }
        break;
        case bf::COND_JMP_START:
        {
            track_jmp_ins.push(collected_ops.size());
            collected_ops.emplace_back(op, 0);
        }
        break;
        case bf::COND_JMP_END:
        {
            if (not track_jmp_ins.empty())
            {
                auto const &jmp_start_ins = track_jmp_ins.top();
                collected_ops.emplace_back(op, jmp_start_ins);
                collected_ops[jmp_start_ins].operand = collected_ops.size() - 1;
                track_jmp_ins.pop();
            }
            else
            {
                Panic("Token parsing error: unmatched `]`");
            }
        }
        break;
        default:
            // no-op code comment
            continue;
        }
    }

    if (not track_jmp_ins.empty())
        Panic("Token parsing error: unmatched `[`");

    return collected_ops;
}

void bf::preproc::print_bf_tokens(std::vector<ops> &tokens)
{
    std::cout << "*** printing tokens ***" << std::endl;
    for (const auto c : tokens)
    {
        std::cout << "[" << g_INS_to_str.at(c.op) << " " << c.operand << "]\n";
    }
    std::cout << std::endl;
}
