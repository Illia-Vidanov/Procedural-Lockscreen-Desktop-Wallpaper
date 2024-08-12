#include "GenerateImage.hpp"

#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <string>
#include <exception>
#include <cstring>

#include "nlohmann/json.hpp"
#include "random/random.hpp"

#include "FlagParser.hpp"
#include "Api.hpp"
#include "CreateImage.hpp"
#include "LuaParser.hpp"
#include "Constants.hpp"
#include "JsonParser.hpp"
#include "StringUtils.hpp"


void GenerateImage(const Flags &flags)
{
    std::shared_ptr<nlohmann::json> json;

    if(!std::filesystem::exists(kJsonName))
    {
        json = InitializeJson(flags);
        std::cout << kJsonName << " generated";
    }
    else
        json = GetJsonFile(kJsonName);

    ImageData image_data = GetImageDataFromJson(json);
    std::cout << "Data from json was succesfully read\n";

    std::vector<std::string> scripts = GatherScripts();

    if(scripts.size() == 0)
    {
        GenerateBlanckImage(image_data);
        std::cout << "No scripts found, blanck image generated\n";
    }

    int index = effolkronium::random_static::get(0, static_cast<int>(scripts.size()) - 1);
    if(scripts.size() > 1 && GetLastScriptFromJson(json) == scripts[index])
    {
        std::swap(scripts[index], scripts.back());
        index = effolkronium::random_static::get(0, static_cast<int>(scripts.size()) - 2);
    }

    (*json)[kLastScriptKey] = scripts[index];
    WriteJsonToFile(json, kJsonName);
    std::cout << "Script choosen: \"" << scripts[index] << "\"\n";

    ProcessScript(image_data, scripts[index]);
    std::cout << "Succesfully created image at \'" << image_data.path << "\' with resolution: " << image_data.width << 'x' << image_data.height << '\n';
}

void GenerateBlanckImage(const ImageData &image_data)
{
    const int data_size = GetStride(image_data.width) * image_data.height;
    uint8_t *data = new uint8_t[data_size];
    std::fill(data, data + data_size, 255);

    CreateImage(image_data, data);

    delete[] data;
    return;
}

std::vector<std::string> GatherScripts()
{
    std::vector<std::string> scripts;
    for(const auto &dir_entry : std::filesystem::recursive_directory_iterator(AddCurrentPathToString(kScriptsFolderName)))
    {
        const auto &ext = dir_entry.path().extension();
        if(ext == ".exe" || ext == ".lua")
            scripts.emplace_back(dir_entry.path().generic_u8string());
    }

    return scripts;
}

void ProcessScript(const ImageData &image_data, const std::string &script)
{
    switch(script.back())
    {
    // .exe file
    case 'e':
        ProcessExe(image_data, script);
        break;
    // .lua file
    case 'a':
        ProcessLua(image_data, script);
        break;
    default:
        std::cout << "Some ultra rare error in process of executing script";
        break;
    }
}

void ProcessLua(const ImageData &image_data, const std::string &script)
{
    const auto lua = InitializeLua();

    CreateImage(image_data, ExecuteLua(lua, image_data.width, image_data.height, script));
    PopReturnLua(lua);
    
    // lua handles memory so no need to delete return of 'ExecuteLua'
    ExitLua(lua);
}

void ProcessExe(const ImageData &image_data, const std::string &script)
{
    std::string args(std::to_string(image_data.width) + " " + std::to_string(image_data.height) + " \"" + image_data.path + '\"');
    int exit_code;
    if((exit_code = ExecuteProgram(script.c_str(), &args[0])))
    {
        std::cout << "Error executing " << script << '\n'
                  << "Exit code: " <<  exit_code << '\n'
                  << "Terminating";
        std::terminate();
    }
}