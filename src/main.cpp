#include <iostream>
#include "bf.hpp"

int main(int argc, char *argv[])
{
    if(not bf::detail::is_bf_file(argc, argv))
    {
        return 1;
    }

    std::vector<char> v = bf::detail::parse_bf_tokens(argv);
    for (const char c : v)
    {
        std::cout << c;
    }
    std::cout << std::endl;
    return 0;
}
