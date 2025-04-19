#include "bf.hpp"
#include "defer.hpp"
#include "istream_iterator_impl.hpp"
#include <cstring>
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

static bool is_input_ok(int argc, char **argv)
{
    if (argc <= 1 or argc > 2)
    {
        std::cout << "wrong input size" << std::endl;
        std::cout << "expected " << argv[0] << " <file_name.bf> " << std::endl;
        return false;
    }
    return true;
}

static bool check_regular_file(const char *file)
{
    if (not fs::exists(file))
    {
        std::cerr << "given file:: " << file << " does not exist" << std::endl;
        return false;
    }

    if (not fs::is_regular_file(file))
    {
        std::cerr << "given file:: " << file << " is not a regular file"
                  << std::endl;
        return false;
    }
    return true;
}

static bool contains_bf_extension(const char *argv)
{
    const std::string this_file_extension = fs::path(argv).extension().string();
    if (this_file_extension.empty())
    {
        std::cerr << "no file extension provided" << std::endl;
        return false;
    }

    for (const std::string &valid_extension : BF_EXTENSIONS)
    {
        if (valid_extension == this_file_extension)
            return true;
    }

    std::cerr << "file extension :: " << this_file_extension
              << " is not one of recognized extensions:: ";
    for (const std::string &valid_extension : BF_EXTENSIONS)
    {
        std::cerr << valid_extension << ' ';
    }
    std::cerr << std::endl;

    return false;
}

bool bf::detail::is_bf_file(int &argc, char **&argv)
{
    if (not is_input_ok(argc, argv))
        return false;

    SHIFT_ARGS(argc, argv);

    if (not check_regular_file(argv[0]))
        return false;

    if (not contains_bf_extension(argv[0]))
        return false;

    return true;
}

std::vector<char> bf::detail::parse_bf_tokens(char **argv)
{
    std::ifstream bf_file(argv[0]);
    defer { bf_file.close(); };
    return {bf::istream_iterator(bf_file), bf::istream_iterator()};
}
