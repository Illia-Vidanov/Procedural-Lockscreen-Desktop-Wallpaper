#include "GenerateImage.hpp"

#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <string>
#include <cstring>

#include "random/random.hpp"

#include "FlagParser.hpp"
#include "Api.hpp"
#include "CreateImage.hpp"
#include "LuaParser.hpp"
#include "Constants.hpp"
#include "JsonFunctions.hpp"

void GenerateImage(const char **arg_begin, const char **arg_end)
{
    if(!std::filesystem::exists(kJsonName))
    {
        InitializeJson(arg_begin, arg_end);
        std::cout << kJsonName << " generated";
    }

    ImageData image_data = GetImageDataFromJson(kJsonName);
    std::cout << "Data from json was succesfully read\n";

    std::vector<std::string> scripts = GatherScripts();

    if(scripts.size() == 0)
    {
        std::cout << "No scripts found, generating blanck image\n";
        GenerateBlanckImage(image_data);
    }

    const std::string &script = scripts[effolkronium::random_static::get(0, static_cast<int>(scripts.size()) - 1)];
    std::cout << "Script choosen: \"" << script << "\"\n";

    ProcessScript(image_data, script);
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
    for(const auto &dir_entry : std::filesystem::recursive_directory_iterator("scripts"))
    {
        const auto &ext = dir_entry.path().extension();
        if(ext == ".exe" || ext == ".lua")
            scripts.emplace_back(dir_entry.path().string());
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
        break;;
    }
}

void ProcessLua(const ImageData &image_data, const std::string &script)
{
    const auto lua = InitializeLua();

    CreateImage(image_data, AddPadding(image_data.width, image_data.height, ExecuteLua(lua, image_data.width, image_data.height, script)));
    PopReturnLua(lua);
    
    // lua handles memory so no need to delete return of 'ExecuteLua'
    ExitLua(lua);
}

void ProcessExe(const ImageData &image_data, const std::string &script)
{
    std::string args(std::to_string(image_data.width) + " " + std::to_string(image_data.height) + " \"" + image_data.path + '\"');
    ExecuteProgram(script.c_str(), &args[0]);
}