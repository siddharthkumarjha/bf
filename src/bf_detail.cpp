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
    if (argc <= 1)
    {
        std::cout << "wrong input size" << std::endl;
        std::cout << "expected " << argv[0] << " [--debug] <file_name.bf>"
                  << std::endl;
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

    for (const std::string &valid_extension : bf::BF_EXTENSIONS)
    {
        if (valid_extension == this_file_extension)
            return true;
    }

    std::cerr << "file extension :: " << this_file_extension
              << " is not one of recognized extensions:: ";
    for (const std::string &valid_extension : bf::BF_EXTENSIONS)
    {
        std::cerr << valid_extension << ' ';
    }
    std::cerr << std::endl;

    return false;
}

static bool is_bf_file(int &argc, char **&argv)
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

static bool is_debug_flag_on(int &argc, char **&argv)
{
    bool debug_flag = false;
    if (argc <= 1)
    {
        Panic("wrong input size\n", "expected ", argv[0],
              " [--debug] <file_name.bf>\n");
    }

    if (argv[1][0] == '-' && argv[1][1] == '-')
    {
        const size_t n = strlen(argv[1] + 2);
        const size_t m = strlen("debug");
        if ((n == m) && strcmp(argv[1] + 2, "debug") == 0)
        {
            debug_flag = true;
        }
        else
        {
            Panic("unknown flags passed\nExpected ", argv[0],
                  " [--debug] <file_name.bf>\n");
        }

        --argc;
        if (argc <= 1)
        {
            Panic("Size of input is small\nExpected ", argv[0],
                  " [--debug] <file_name.bf>\n");
        }

        argv[1] = argv[2];
    }
    return debug_flag;
}

std::pair<bool, bool> bf::detail::parse_cmd_line(int &argc, char **&argv)
{
    const bool debug_flag      = is_debug_flag_on(argc, argv);
    const bool is_bf_file_flag = is_bf_file(argc, argv);

    return {debug_flag, is_bf_file_flag};
}

std::vector<char> bf::detail::parse_bf_tokens(char **argv)
{
    std::ifstream bf_file(argv[0]);
    defer { bf_file.close(); };
    return {bf::istream_iterator(bf_file), bf::istream_iterator()};
}

void bf::detail::print_bf_tokens(std::vector<char> &tokens)
{
    std::cout << "*** printing tokens ***" << std::endl;
    for (const auto c : tokens)
    {
        std::cout << c;
    }
    std::cout << '\n' << std::endl;
}
