#include <iostream>

#include "Setup.hpp"
#include "FlagParser.hpp"

int main(int argc, const char **argv)
{
    Flags flags(argc, argv);
    Setup(flags);

    return 0;
}