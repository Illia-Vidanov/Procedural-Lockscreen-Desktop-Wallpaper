// Most simple example of a program

#include <iostream>
#include <stdint.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int RandomInRange(int min, int max) // range: [min, max]
{
	return min + std::rand() % (( max + 1 ) - min);
}

int main(int argc, char **argv)
{
	std::srand(std::time(NULL));
	
	int width = std::atoi(argv[0]);
	int height = std::atoi(argv[1]);
	const char *path = argv[2];
	
	const int red = RandomInRange(0, 255);
	const int green = RandomInRange(0, 255);
	const int blue = RandomInRange(0, 255);
	
	const int data_length = width * 3 * height;
	uint8_t *data = new uint8_t[data_length];
	for(int i = 0; i < data_length; i += 3)
	{
		data[i + 0] = red;
		data[i + 1] = green;
		data[i + 2] = blue;
	}
	
	stbi_write_png(path, width, height, 3, data, width * 3);
	
	return 0;
}