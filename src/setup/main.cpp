#include <iostream>

#include "Setup.hpp"

int main(int argc, const char **argv)
{
    Setup(argv, argv + argc);

    std::cout << "Everything is setup\n";
    std::cout << "\nPress any button to close...";
    std::cin.ignore();

    return 0;
}