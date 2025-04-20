#include "bf.hpp"
#include "defer.hpp"
#include "fstream"
#include "iostream"
#include "istream_iterator_impl.hpp"

std::vector<char> bf::preproc::parse_bf_tokens(const std::string &file_name)
{
    if (file_name.empty())
        Panic("No input file found");

    std::ifstream bf_file(file_name);
    defer { bf_file.close(); };
    return {bf::istream_iterator(bf_file), bf::istream_iterator()};
}

void bf::preproc::print_bf_tokens(std::vector<char> &tokens)
{
    std::cout << "*** printing tokens ***" << std::endl;
    for (const auto c : tokens)
    {
        std::cout << c;
    }
    std::cout << '\n' << std::endl;
}
