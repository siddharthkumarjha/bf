#include <filesystem>
#include <iostream>
#include <vector>
#include "istream_iterator_impl.hpp"
#include "bf.hpp"

namespace fs = std::filesystem;

#define SHIFT_ARGS(argc, argv)                                                 \
    do                                                                         \
    {                                                                          \
        --argc;                                                                \
        ++argv;                                                                \
    } while (false)

void usage(const char *file)
{
    std::cout << "expected " << file << " <your_file.bf> " << std::endl;
}

int check_regular_file(const char *file)
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

int main(int argc, char *argv[])
{
    if (argc <= 1 or argc > 2)
    {
        usage(argv[0]);
        return 1;
    }
    SHIFT_ARGS(argc, argv);

    if (check_regular_file(argv[0]) > 0)
        return 1;

    std::ifstream bf_file(argv[0]);
    std::vector<char> v{istream_iterator(bf_file), istream_iterator()};
    bf_file.close();
    for (const char c : v)
    {
        std::cout << c;
    }
    std::cout << std::endl;
    return 0;
}
