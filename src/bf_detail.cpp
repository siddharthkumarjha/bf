#include "bf.hpp"
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

#define SHIFT_ARGS(argc, argv)                                                 \
    do                                                                         \
    {                                                                          \
        --argc;                                                                \
        ++argv;                                                                \
    } while (false)

static void panic_if_not_regular_file(const char *file)
{
    if (not fs::exists(file))
    {
        Panic("given file:: ", file, " does not exist");
    }

    if (not fs::is_regular_file(file))
    {
        Panic("given file:: ", file, " is not a regular file");
    }
}

static void panic_if_not_bf_ext(const char *file)
{
    const std::string this_file_extension = fs::path(file).extension().string();
    if (this_file_extension.empty())
    {
        Panic("no file extension provided");
    }

    for (const std::string &valid_extension : bf::BF_EXTENSIONS)
    {
        if (valid_extension == this_file_extension)
            return;
    }

    Panic("file extension:: ", this_file_extension,
          "\n\t\tis not one of recognized extensions:: ", bf::BF_EXTENSIONS);
}

static void panic_if_not_bf_file_name(const char *file_name)
{
    panic_if_not_regular_file(file_name);
    panic_if_not_bf_ext(file_name);
}

std::pair<bool, std::string> bf::detail::parse_cmd_line(int &argc, char **&argv)
{
    const char *file_name = argv[0];
    SHIFT_ARGS(argc, argv);

    if (argc <= 0)
        Panic("Size of input is small\nExpected ", file_name,
              " [--debug] <file_name.bf>\n");

    if (argc > 2)
        Panic("Too many args passed\nExpected ", file_name,
              " [--debug] <file_name.bf>\n");

    bool debug_flag = false;
    std::string bf_file_name;

    while (argc > 0)
    {
        if (strncmp(argv[0], "--", 2) == 0)
        {
            if (strcmp(argv[0], "--debug") == 0)
                debug_flag = true;
            else
                Panic("Unknown flag passed. Only [--debug] expected");
        }
        else
        {
            panic_if_not_bf_file_name(argv[0]);
            if (bf_file_name.empty())
                bf_file_name = argv[0];
            else
                Panic("Too many bf source files given only 1 expected");
        }
        SHIFT_ARGS(argc, argv);
    }

    return {debug_flag, bf_file_name};
}
