#include <iostream>

#include "Setup.hpp"

int main(int argc, const char **argv)
{
    Setup(argv, argv + argc);

    std::cout << "Everything is setup\n";

    return 0;
}