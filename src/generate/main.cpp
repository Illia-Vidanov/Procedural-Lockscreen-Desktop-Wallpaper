#include <iostream>
#include <windows.h>

#include "GenerateImage.hpp"
#include "Api.hpp"

int main(int argc, const char **argv)
{
    GenerateImage(argv, argv + argc);
    
    std::cout << "Image generated" << std::endl;
    MySleep(1);
    return 0;
}