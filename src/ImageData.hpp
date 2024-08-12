#ifndef IMAGE_DATA_HPP
#define IMAGE_DATA_HPP

#include <string>


// Struct with width, height and image path
struct ImageData
{
    inline ImageData(int image_width, int image_height, const std::string &image_path) : width(image_width), height(image_height), path(image_path) {}
    inline ImageData() = default;

    int width = 0;
    int height = 0;
    std::string path;
};

#endif // IMAGE_DATA_HPP