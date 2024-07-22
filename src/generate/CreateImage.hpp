#ifndef CREATE_IMAGE_HPP
#define CREATE_IMAGE_HPP

#include <string>
#include <stdint.h>

#include "ImageData.hpp"

constexpr inline int kImageDepth = 3;
constexpr inline int kImageStrideMultipleOf = 8;

void CreateImage(const ImageData &image_data, const void *data);
auto AddPadding(const int width, const int height, const void *data) -> void*;
auto GetStride(const int width) -> int;

#endif // CREATE_IMAGE_HPP