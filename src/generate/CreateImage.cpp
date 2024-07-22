#include "CreateImage.hpp"

#include <stdint.h>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Winapi.hpp"
#include "ImageData.hpp"

void CreateImage(const ImageData &image_data, const void *data)
{
    if(!stbi_write_png(image_data.path.c_str(), image_data.width, image_data.height, kImageDepth, data, GetStride(image_data.width)))
    {
        std::cout << "Error creating png at " << image_data.path << '\n'
                  << "Try running again\n";
        std::terminate();
    }

    std::cout << "Succesfully created image at \'" << image_data.path << "\' with resolution: " << image_data.width << 'x' << image_data.height << '\n';
}

auto AddPadding(const int width, const int height, const void *data) -> void*
{
    const int kStride = GetStride(width);
    const int kDataRow = width * kImageDepth;
	
	// If already multiple of kStride
	if(kDataRow == kStride)
	{
		const int kDataSize = kStride * height;
		uint8_t *new_data = new uint8_t[kDataSize];
		std::memcpy(new_data, data, kDataSize);
		
		return new_data;
	}
	
    const int kNewDataSize = kStride * height;
    uint8_t *new_data = new uint8_t[kNewDataSize];
    std::fill(new_data, new_data + kNewDataSize, 0);

    for(int i = 0; i < height; i++)
        std::memcpy(new_data + kStride * i, reinterpret_cast<const uint8_t*>(data) + kDataRow * i, kDataRow);

    return new_data;
}

auto GetStride(const int width) -> int
{
    const int kByteWidth = width * kImageDepth;
    return kByteWidth + kImageStrideMultipleOf - (kByteWidth % kImageStrideMultipleOf);
}