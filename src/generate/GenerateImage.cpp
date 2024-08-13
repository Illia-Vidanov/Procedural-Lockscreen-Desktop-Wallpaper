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
#include "ScriptManager.hpp"


void GenerateImage(const Flags &flags, const ScriptManager &script_manager)
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

    const ScriptManager::MapType::value_type *key_value = script_manager.GetRandom(GetLastScriptFromJson(json));

    if(key_value == nullptr)
    {
        GenerateBlanckImage(image_data);
        std::cout << "No scripts found, blank image generated\n";
        return;
    }

    std::string script = key_value->second.generic_u8string();
    std::cout << "Script choosen: \"" << script << "\"\n";

    (*json)[kLastScriptKey] = key_value->first;
    WriteJsonToFile(json, kJsonName);

    ProcessScript(image_data, script);
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