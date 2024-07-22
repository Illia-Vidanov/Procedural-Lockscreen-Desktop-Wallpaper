#ifndef JSON_FUNCTIONS_HPP
#define JSON_FUNCTIONS_HPP

#include <string>

#include "ImageData.hpp"

void InitializeJson(const char **args_begin, const char **args_end);
auto GetImageDataFromJson(const std::string &json_path) -> ImageData;
auto AddCurrentPathToString(const std::string &str) -> std::string;

#endif // JSON_FUNCTIONS_HPP