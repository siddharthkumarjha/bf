#include "bf.hpp"
#include "streamable_array.hpp"
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

#define SHIFT_ARGS(argc, argv)                                                 \
    do                                                                         \
    {                                                                          \
        --argc;                                                                \
        ++argv;                                                                \
    } while (false)

static void panic_if_not_regular_file(fs::path const &file)
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

static void panic_if_not_bf_ext(fs::path const &file)
{
    const std::string_view this_file_extension =
        fs::path(file).extension().c_str();

    if (this_file_extension.empty())
    {
        Panic("no file extension provided");
    }

    for (const auto &valid_extension : bf::BF_EXTENSIONS)
    {
        if (valid_extension == this_file_extension)
            return;
    }

    Panic("file extension:: ", this_file_extension,
          "\n\t\tis not one of recognized extensions:: ", bf::BF_EXTENSIONS);
}

static void panic_if_not_bf_file_name(fs::path file_name)
{
    panic_if_not_regular_file(file_name);
    panic_if_not_bf_ext(file_name);
}

bf::Cli_Options bf::detail::parse_cmd_line(int &argc, char **&argv)
{
    const std::string_view prog_name = argv[0];
    SHIFT_ARGS(argc, argv);

    if (argc <= 0)
        Panic("Size of input is small\nExpected ", prog_name, ' ',
              bf::EXPECTED_FLAGS, " <file_name.bf>\n");

    if (static_cast<size_t>(argc) > bf::EXPECTED_FLAGS.size())
        Panic("Too many args passed\nExpected ", prog_name, ' ',
              bf::EXPECTED_FLAGS, " <file_name.bf>\n");

    struct Cli_Options opts;

    while (argc > 0)
    {
        if (strncmp(argv[0], "--", 2) == 0)
        {
            auto cur_itr       = bf::EXPECTED_FLAGS.begin();
            auto const end_itr = bf::EXPECTED_FLAGS.end();
            for (; cur_itr != end_itr; ++cur_itr)
            {
                if (*cur_itr == argv[0])
                {
                    opts[static_cast<FLAG>(cur_itr -
                                           bf::EXPECTED_FLAGS.begin())] = true;
                    break;
                }
            }
            if (cur_itr == end_itr)
                Panic("Unknown flag ", argv[0], " passed, expected ",
                      bf::EXPECTED_FLAGS);
        }
        else
        {
            panic_if_not_bf_file_name(argv[0]);
            if (opts.file_name.empty())
                opts.file_name = argv[0];
            else
                Panic("Too many bf source files given only 1 expected");
        }
        SHIFT_ARGS(argc, argv);
    }

    return opts;
}
