#include "bf.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
    try
    {
        auto opts = bf::parse_cmd_line(argc, argv);

        if (!opts[bf::COMPILE_FLAG] && !opts[bf::INTERPRET_FLAG])
            opts[bf::INTERPRET_FLAG] = true;

        std::vector<bf::ops> tokens = bf::parse_tokens(opts.file_name);

        if (opts[bf::DEBUG_FLAG])
            bf::print_tokens(tokens);

        if (opts[bf::INTERPRET_FLAG])
            bf::interpret(tokens, opts[bf::DEBUG_FLAG]);

        if (opts[bf::COMPILE_FLAG])
            bf::compile(tokens, opts[bf::DEBUG_FLAG]);
    }
    catch (std::exception const &excuse)
    {
        std::cerr << excuse.what() << '\n' << std::flush;
        std::cerr << "exiting..." << '\n' << std::flush;

        return 1;
    }
    return 0;
}
