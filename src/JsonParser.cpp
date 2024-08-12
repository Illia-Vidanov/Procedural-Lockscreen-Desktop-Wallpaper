#include "JsonParser.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <exception>

#include "nlohmann/json.hpp"

#include "Api.hpp"
#include "Constants.hpp"
#include "FlagParser.hpp"
#include "ImageData.hpp"
#include "StringUtils.hpp"


auto InitializeJson(const Flags &flags) -> std::shared_ptr<nlohmann::json>
{
    int width = std::atoi(flags.Get(kWidthFlag).c_str());
    int height = std::atoi(flags.Get(kHeightFlag).c_str());

    if(width <= 0 || height <= 0)
        GetScreenResolution(width, height);
    
    std::shared_ptr<nlohmann::json> json =
    std::make_shared<nlohmann::json>(nlohmann::json::initializer_list_t{
        { kPathKey, AddCurrentPathToString(kImageName) },
        { kWidthKey, width },
        { kHeightKey, height },
        { kLastScriptKey, "" }
    });

    WriteJsonToFile(json, kJsonName);

    std::cout << kJsonName << " generated\n";

    return json;
}

auto GetImageDataFromJson(std::shared_ptr<nlohmann::json> json) -> ImageData
{
    int width;
    int height;
    std::string path;
    //bool changed = false;

    if(!(*json).contains(kWidthKey) || !(*json).contains(kHeightKey))
    {
        //changed = true;
        std::cout << "Width or height was not found in json\n";
        GetScreenResolution(width, height);    
        (*json)[kWidthKey] = width;
        (*json)[kHeightKey] = height;
        std::cout << "Set resolution to " << width << 'x' << height << '\n';
    }
    else
    {
        width = (*json)[kWidthKey].get<nlohmann::json::number_integer_t>();
        height = (*json)[kHeightKey].get<nlohmann::json::number_integer_t>();
    }

    if(!(*json).contains(kPathKey))
    {
        //changed = true;
        std::cout << "Path was not found in json\n";
        (*json)[kPathKey] = path = AddCurrentPathToString(kImageName);
        std::cout << "Set path to " << path << '\n';
    }
    else
        path = (*json)[kPathKey].get<nlohmann::json::string_t>();

    // Json will be rewritten in GenerateImage to set LastScript so no need to do it here
    //if(changed)
    //    WriteJsonToFile(json);

    return ImageData(width, height, path);
}

auto GetLastScriptFromJson(std::shared_ptr<nlohmann::json> json) -> std::string
{
    // It will be inserted and written to file anyways in GenerateImage
    if(!(*json).contains(kLastScriptKey))
        return "";

    return (*json)[kLastScriptKey].get<nlohmann::json::string_t>();
}

auto GetJsonFile(const std::string &json_path) -> std::shared_ptr<nlohmann::json>
{
    std::ifstream file(json_path);
    if (!file.is_open())
    {
        std::cout << "Failed to open json file. Terminating";
        std::terminate();
    }

    std::shared_ptr<nlohmann::json> json = std::make_shared<nlohmann::json>(nlohmann::json::parse(file));

    file.close();

    return json;
}

void WriteJsonToFile(std::shared_ptr<nlohmann::json> json, const std::string &path)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        std::cout << "Failed to open json file. Terminating";
        std::terminate();
    }

    file << std::setw(4) << (*json);
    file.close();
}