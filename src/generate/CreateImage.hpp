#ifndef CREATE_IMAGE_HPP
#define CREATE_IMAGE_HPP

#include <string>
#include <stdint.h>

#include "ImageData.hpp"


void CreateImage(const ImageData &image_data, const void *data);
auto AddPadding(const int width, const int height, const void *data) -> uint8_t*;
auto GetStride(const int width) -> int;

#endif // CREATE_IMAGE_HPP