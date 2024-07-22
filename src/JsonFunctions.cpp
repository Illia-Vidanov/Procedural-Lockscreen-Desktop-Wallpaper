#include "JsonFunctions.hpp"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

#include "nlohmann/json.hpp"

#include "Api.hpp"
#include "Constants.hpp"
#include "FlagParser.hpp"
#include "ImageData.hpp"

void InitializeJson(const char **args_begin, const char **args_end)
{
    int width = std::atoi(GetFlag(args_begin, args_end, "-w").c_str());
    int height = std::atoi(GetFlag(args_begin, args_end, "-h").c_str());

    if(width <= 0 || height <= 0)
        GetScreenResolution(width, height);
    
    nlohmann::json json =
    {
        { kPathKey, AddCurrentPathToString(kImageName) },
        { kWidthKey, width },
        { kHeightKey, height }
    };

    std::ofstream file(kJsonName);
    if (!file.is_open())
    {
        std::cout << "\n Failed to open json file";
        return;
    }

    file << std::setw(4) << json;
    file.close();
}

ImageData GetImageDataFromJson(const std::string &json_path)
{
    std::ifstream file(json_path);
    if (!file.is_open())
    {
        std::cout << "\n Failed to open json file. Terminating";
        std::terminate();
    }

    nlohmann::json json = nlohmann::json::parse(file);
     
    return ImageData(json[kWidthKey].get<nlohmann::json::number_integer_t>(), json[kHeightKey].get<nlohmann::json::number_integer_t>(), json[kPathKey].get<nlohmann::json::string_t>());
}

auto AddCurrentPathToString(const std::string &str) -> std::string
{
    return std::filesystem::current_path().string() + "\\" + str;
}