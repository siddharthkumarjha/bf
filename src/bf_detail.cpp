#include "bf.hpp"
#include "defer.hpp"
#include "istream_iterator_impl.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

#define SHIFT_ARGS(argc, argv)                                                 \
    do                                                                         \
    {                                                                          \
        --argc;                                                                \
        ++argv;                                                                \
    } while (false)

static void usage(const char *file)
{
    std::cout << "expected " << file << " <your_file.bf> " << std::endl;
}

static int check_regular_file(const char *file)
{
    if (not fs::exists(file))
    {
        std::cerr << "given file:: " << file << " does not exist" << std::endl;
        return 1;
    }

    if (not fs::is_regular_file(file))
    {
        std::cerr << "given file:: " << file << " is not a regular file"
                  << std::endl;
        return 1;
    }
    return 0;
}

bool bf::detail::is_bf_file(int &argc, char **&argv)
{
    if (argc <= 1 or argc > 2)
    {
        usage(argv[0]);
        return false;
    }
    SHIFT_ARGS(argc, argv);

    if (check_regular_file(argv[0]) > 0)
        return false;

    return true;
}

std::vector<char> bf::detail::parse_bf_tokens(char **argv)
{
    std::ifstream bf_file(argv[0]);
    defer { bf_file.close(); };
    return {bf::istream_iterator(bf_file), bf::istream_iterator()};
}
