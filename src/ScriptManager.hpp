#ifndef SCRIPT_MANAGER_HPP
#define SCRIPT_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <filesystem>


class Flags;

class ScriptManager
{
public:
    using MapType = std::unordered_map<std::string, std::filesystem::path>;    

    ScriptManager();

    auto Check(const Flags &flags) -> bool;
    auto GetRandom(const std::string &last_script) const -> const MapType::value_type*;

private:
    MapType scripts;
    MapType dis_scripts;

    void List();
    auto DisableScript(const MapType::const_iterator &it) -> bool;
    auto EnableScript(const MapType::const_iterator &it) -> bool;
    auto DeleteScript(const MapType::const_iterator &it, MapType &map) -> bool;
};

#endif // SCRIPT_MANAGER_HPP