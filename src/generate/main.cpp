#include <iostream>
#include <windows.h>

#include "GenerateImage.hpp"
#include "FlagParser.hpp"

int main(int argc, const char **argv)
{
    Flags flags(argc, argv);
    GenerateImage(flags);

    return 0;
}