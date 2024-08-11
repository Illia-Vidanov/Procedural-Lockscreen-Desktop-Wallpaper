#include <iostream>
#include <stdint.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <vector>
#include <utility>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Constants
constexpr inline int kImageDepth = 3;
constexpr inline int kImageStrideMultipleOf = 8;

// values are in pixels
constexpr inline int kMinHorCircles = 20;
constexpr inline int kMaxHorCircles = 30;

constexpr inline int kMinVerCircles = 15;
constexpr inline int kMaxVerCircles = 25;

constexpr inline int kMinCircleRadius = 40;
constexpr inline int kMaxCircleRadius = 80;


// Forward declarations

struct Color;

inline auto RandomInRange(const int min, const int max) -> int;
inline auto GetRandomColor() -> Color;
inline constexpr auto GetStride(const int width) -> int;


// Defenitions

struct Color
{
    constexpr Color() = default;
    constexpr Color(uint8_t red_part, uint8_t green_part, uint8_t blue_part) : red(red_part), green(green_part), blue(blue_part) {}

    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
};

struct Circle
{
	constexpr Circle() = default;
	constexpr Circle(int centr_x, int centr_y, int circle_radius, Color circle_color = GetRandomColor()) : x(centr_x), y(centr_y), radius(circle_radius), color(circle_color) {}

	int x = 0, y = 0;
	int radius = 0;
	Color color;
};

inline int RandomInRange(const int min, const int max) // range: [min, max]
{
	return min + std::rand() % (( max + 1 ) - min);
}

inline auto GetRandomColor() -> Color
{
    return Color(RandomInRange(0, 255), RandomInRange(0, 255), RandomInRange(0, 255));
}

inline auto AddPadding(const int width, const int height, const void *data) -> void*
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

inline constexpr auto GetStride(const int width) -> int
{
    const int kByteWidth = width * kImageDepth;
    return kByteWidth + kImageStrideMultipleOf - (kByteWidth % kImageStrideMultipleOf);
}

// Written as single line, because I remember reading that compiler is more likely to inline such functions and inlining this function is really important
inline constexpr void PutColor(const int index, const Color &color, uint8_t *data) { data[index + 0] = color.red; data[index + 1] = color.green; data[index + 2] = color.blue; }

inline void FillWithRandomColor(const int data_length, uint8_t *data)
{
    const Color color = GetRandomColor();

	for(int i = 0; i < data_length; i += kImageDepth)
		PutColor(i, color, data);
}

auto GenerateCircles(const int width, const int height) -> std::vector<Circle>
{
	int ver_circles = RandomInRange(kMinHorCircles, kMaxHorCircles);
	int hor_circles = RandomInRange(kMinVerCircles, kMaxVerCircles);

	if(ver_circles > hor_circles)
		std::swap(ver_circles, hor_circles);

	std::vector<Circle> circles(hor_circles * ver_circles);

	// division with ceiling for future bounds check
	// It won't work(some of the circles will be skipped) on small screen resolutions or in case there are a lot of circles, but it's good enough for me I guess
	// By the way, everything would be normal if I have used floats
	const int const_hor_offset = (width + hor_circles) / (hor_circles + 1);
	const int const_ver_offset = (height + ver_circles) / (ver_circles + 1);
	
	int i = 0;
	for(int ver_offset = const_ver_offset; ver_offset < height; ver_offset += const_ver_offset)
	{
		for(int hor_offset = const_hor_offset; hor_offset < width; hor_offset += const_hor_offset)
		{
			circles[i] = Circle(hor_offset, ver_offset, RandomInRange(kMinCircleRadius, kMaxCircleRadius), GetRandomColor());
			i++;
		}
	}

	return circles;
}

void DrawCircle(int width, int height, uint8_t *data, const Circle &circle)
{
	const int radius_squared = circle.radius * circle.radius;

	for(int y = std::max(0, circle.y - circle.radius); y < std::min(height, circle.y + circle.radius); y++)
	{
		for(int x = std::max(0, circle.x - circle.radius); x < std::min(width, circle.x + circle.radius); x++)
		{
			if(std::pow(std::abs(circle.x - x), 2) + std::pow(std::abs(circle.y - y), 2) < radius_squared)
				PutColor((x + y * width) * kImageDepth, circle.color, data);
		}
	}
}

int main(int argc, char **argv)
{
	std::srand(std::time(NULL));
	
	int width = std::atoi(argv[0]);
	int height = std::atoi(argv[1]);
	const char *path = argv[2];

    const int data_length = width * kImageDepth * height;
	uint8_t *data = new uint8_t[data_length];

    FillWithRandomColor(data_length, data);

    std::vector<Circle> circles = GenerateCircles(width, height);
    // Make them in random order so overlap isn't very ordered
	std::random_shuffle(circles.begin(), circles.end());
	for(std::size_t i = 0; i < circles.size(); i++)
		DrawCircle(width, height, data, circles[i]);
	
	stbi_write_png(path, width, height, kImageDepth, AddPadding(width, height, data), GetStride(width));
	
	return 0;
}