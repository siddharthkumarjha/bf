#include "bf.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
    try
    {
        auto [debug_flag, bf_file_name] = bf::parse_cmd_line(argc, argv);

        std::vector<bf::ops> tokens = bf::parse_bf_tokens(bf_file_name);

        if (debug_flag)
            bf::print_bf_tokens(tokens);

        bf::interpret(tokens, debug_flag);
    }
    catch (std::exception const &excuse)
    {
        std::cerr << excuse.what() << '\n' << std::flush;
        std::cerr << "exiting..." << '\n' << std::flush;

        return 1;
    }
    return 0;
}
