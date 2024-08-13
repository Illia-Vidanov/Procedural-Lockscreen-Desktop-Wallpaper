#include "ScriptManager.hpp"

#include <vector>
#include <filesystem>
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>

#include "random/random.hpp"

#include "Constants.hpp"
#include "StringUtils.hpp"
#include "FlagParser.hpp"


ScriptManager::ScriptManager()
{
    static const std::size_t exe_hash = std::hash<std::string>()(".exe");
    static const std::size_t lua_hash = std::hash<std::string>()(".lua");
    static const std::size_t dis_exe_hash = std::hash<std::string>()(".exe_dis");
    static const std::size_t dis_lua_hash = std::hash<std::string>()(".lua_dis");

    const std::size_t path_length = std::filesystem::current_path().generic_u8string().size() + StrLen(kScriptsFolderName) + 2; // + 2 for 2 more '/'

    for(const auto &dir_entry : std::filesystem::recursive_directory_iterator(AddCurrentPathToString(std::string(kScriptsFolderName))))
    {
        std::size_t ext_hash = std::hash<std::string>()(dir_entry.path().extension().string());
        if(ext_hash == exe_hash || ext_hash == lua_hash)
        {
            const std::string &path = dir_entry.path().generic_u8string();
            scripts.emplace(path.substr(path_length, path.size() - path_length), path);
        }
        else if(ext_hash == dis_exe_hash || ext_hash == dis_lua_hash)
        {
            const std::string &path = dir_entry.path().generic_u8string();
            dis_scripts.emplace(path.substr(path_length, path.size() - path_length - 4), path); // - 4 for _dis part
        }
    }
}

auto ScriptManager::Check(const Flags &flags) -> bool
{
    bool found = false;

    // Disable
    Flags::RangeType range = flags.GetRange(kScriptDisableFlag);
    for(; range.first != range.second; range.first++)
    {
        found = true;
        const auto &it = scripts.find(range.first->second);
        if(DisableScript(it))
            List();
    }

    // Enable
    range = flags.GetRange(kScriptEnableFlag);
    for(; range.first != range.second; range.first++)
    {
        found = true;
        const auto &it = dis_scripts.find(range.first->second);
        if(EnableScript(it))
            List();
    }

    // Delete
    range = flags.GetRange(kScriptDeleteFlag);
    for(; range.first != range.second; range.first++)
    {
        found = true;
        const auto &it = scripts.find(range.first->second);
        if(it == scripts.end())
        {
            if(DeleteScript(dis_scripts.find(range.first->second), dis_scripts))
                List();
            continue;
        }

        if(DeleteScript(it, scripts))
            List();
    }

    if(flags.Contains(kScriptDisableAllFlag))
    {
        found = true;
        for(MapType::const_iterator it = scripts.begin(); it != scripts.end(); it++)
            DisableScript(it);

        List();
    }

    if(flags.Contains(kScriptEnableAllFlag))
    {
        found = true;
        for(MapType::const_iterator it = dis_scripts.begin(); it != dis_scripts.end(); it++)
            EnableScript(it);

        List();
    }

    if(flags.Contains(kScriptDeleteAllFlag))
    {
        found = true;
        for(MapType::const_iterator it = scripts.begin(); it != scripts.end(); it++)
            DeleteScript(it, scripts);
        for(MapType::const_iterator it = dis_scripts.begin(); it != dis_scripts.end(); it++)
            DeleteScript(it, dis_scripts);

        List();
    }

    // List
    if(flags.Contains(kScriptListFlag) && !found)
    {
        found = true;
        List();
    }

    return found;
}

auto ScriptManager::GetRandom(const std::string &last_scritp) const -> const MapType::value_type*
{
    // Hard to tell what container will suit better, on the one hand, we have log(n) lookup in Check() on the other lack of random acces iterator for GetRandom

    if(scripts.size() == 0)
        return nullptr;
    else if(scripts.size() == 1)
        return &*scripts.begin(); 

    const auto &it = scripts.find(last_scritp);
    std::size_t index;
    if(it == scripts.end())
    {
        index = effolkronium::random_static::get<std::size_t>(0, scripts.size() - 1);
    }
    else
    {
        index = effolkronium::random_static::get<std::size_t>(0, scripts.size() - 2);
        if(index >= static_cast<std::size_t>(std::distance(scripts.begin(), it)))
            index++;
    }

    return &*std::next(scripts.begin(), index);
}

void ScriptManager::List()
{
    int largest_name = 0;
    int largest_path = 0;
    for(const auto &it : scripts)
    {
        largest_name = std::max(largest_name, static_cast<int>(it.first.size()));
        largest_path = std::max(largest_path, static_cast<int>(it.second.generic_u8string().size()));
    }
    for(const auto &it : dis_scripts)
    {
        largest_name = std::max(largest_name, static_cast<int>(it.first.size()));
        largest_path = std::max(largest_path, static_cast<int>(it.second.generic_u8string().size()));
    }

    std::cout << "Name " << std::string(std::max(largest_name - 5, 0), ' ') << "  Full Path" << std::string(std::max(largest_path - 9, 0), ' ') << "  Status\n";
    for(const auto &it : scripts)
    {
        const std::string &path = it.second.generic_u8string();
        std::cout << it.first << std::string(largest_name - it.first.size() + 2, ' ') << path << std::string(largest_path - path.size() + 2, ' ') << "enabled\n";
    }
    for(const auto &it : dis_scripts)
    {
        const std::string &path = it.second.generic_u8string();
        std::cout << it.first << std::string(largest_name - it.first.size() + 2, ' ') << path << std::string(largest_path - path.size() + 2, ' ') << "disabled\n";
    }
}

auto ScriptManager::DisableScript(const MapType::const_iterator &it) -> bool
{
    if(it == scripts.end())
    {
        if(dis_scripts.find(it->first) != dis_scripts.end())
        {
            std::cout << "Extension \'" << it->first << "\' already disabled\n";
            return false;
        }

        std::cout << "Extension \'" << it->first << "\' not found\n";
        return false;
    }

    const std::string &path = it->second.generic_u8string();
    std::filesystem::rename(it->second, path + "_dis");
    
    std::cout << "Extension \'" << it->first << "\' disabled\n\n";

    dis_scripts.emplace(it->first, path + "_dis");
    scripts.erase(it);

    return true;
}

auto ScriptManager::EnableScript(const MapType::const_iterator &it) -> bool
{
    if(it == dis_scripts.end())
    {
        if(scripts.find(it->first) != scripts.end())
        {
            std::cout << "Extension \'" << it->first << "\' already enabled\n";
            return false;
        }

        std::cout << "Extension \'" << it->first << "\' not found\n";
        return false;
    }

    const std::string &path = it->second.generic_u8string();
    std::filesystem::rename(it->second, path.substr(0, path.size() - 4));

    std::cout << "Extension \'" << it->first << "\' enabled\n\n";

    scripts.emplace(it->first, path.substr(0, path.size() - 4));
    dis_scripts.erase(it);

    return true;
}

auto ScriptManager::DeleteScript(const MapType::const_iterator &it, MapType &map) -> bool
{    
    if(it == map.end())
    {
        std::cout << "Extension \'" << it->first << "\' not found\n";
        return false;
    }

    std::filesystem::remove(it->second);
    std::cout << "Extension \'" << it->first << "\' deleted\n\n";

    map.erase(it);

    return true;
}