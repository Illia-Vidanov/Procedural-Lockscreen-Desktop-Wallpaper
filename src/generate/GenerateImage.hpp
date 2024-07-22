#ifndef GENERATE_IMAGE_HPP
#define GENERATE_IMAGE_HPP

#include "ImageData.hpp"

#include <string>
#include <vector>

void GenerateImage(const char **arg_begin, const char **arg_end);
void GenerateBlanckImage(const ImageData &image_data);
std::vector<std::string> GatherScripts();
void ProcessScript(const ImageData &image_data, const std::string &script);
void ProcessLua(const ImageData &image_data, const std::string &script);
void ProcessExe(const ImageData &image_data, const std::string &script);

#endif // GENERATE_IMAGE_HPP