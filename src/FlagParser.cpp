#include "FlagParser.hpp"

#include <string>
#include <stdexcept>

// Overall it's pretty lazy approach of implementing this, but it works
// For future it's possible to use better search algorithm with aproximation for the fact that it must be at the start of the

std::string GetFlag(const std::string &str, const std::string &flag)
{
    if(str.find(flag) != 0)
        return "";

    if(flag.length() > str.length())
        return "";

    return str.substr(flag.length(), str.length() - flag.length());
}

std::string GetFlag(const char **args_begin, const char **args_end, const std::string &flag)
{
    for(; args_begin != args_end; args_begin++)
    {
        if(std::string(*args_begin).find(flag) != std::string::npos)
            return GetFlag(*args_begin, flag);
    }

    return "";
}

std::string GetFlag(const char **args_begin, const char **args_end, const std::string &flag, const char ***pos)
{
    for(; args_begin != args_end; args_begin++)
    {
        if(std::string(*args_begin).find(flag) != std::string::npos)
        {
            *pos = args_begin;
            return GetFlag(*args_begin, flag);
        }
    }

    pos = nullptr;
    return "";
}

const char **FlagExists(const char **args_begin, const char **args_end, const std::string &flag)
{
    for(; args_begin != args_end; args_begin++)
    {
        if(std::string(*args_begin).find(flag) == 0)
            return args_begin;
    }

    return nullptr;
}
