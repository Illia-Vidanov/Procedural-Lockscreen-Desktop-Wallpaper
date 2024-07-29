#include "FlagParser.hpp"

#include <string>
#include <stdexcept>



bool StringStartsWith(const std::string &prefix, const std::string &str)
{
    for(std::size_t i = 0; i < prefix.size(); i++)
    {
        if(str[i] != prefix[i])
            return false;
    }

    return true;
}

auto Flags::GetArgsAsString(const std::string &delimetr) const -> std::string
{
    const char **it = begin_;
    std::string result(*it);
    it++;
    while(it != end_)
    {
        result += delimetr;
        result += (*it);
        it++;
    }

    return result;
}

auto Flags::Count(const std::string &flag, const char **args_begin, const char **args_end) -> std::size_t
{
    std::size_t count = 0;

    for(; args_begin != args_end; args_begin++)
        count += StringStartsWith(flag, *args_begin);

    return count;
}

auto Flags::Contains(const std::string &flag, const char **args_begin, const char **args_end) -> const char **
{
    for(; args_begin != args_end; args_begin++)
    {
        if(StringStartsWith(flag, *args_begin))
            return args_begin;
    }

    return nullptr;
}

auto Flags::Get(const std::string &flag, const std::string &str) -> std::string
{
    // Goes first because StringStartsWith do not perform bounds checking
    if(flag.size() >= str.size())
        return "";

    if(StringStartsWith(flag, str))
        return "";

    return str.substr(flag.size(), str.size() - flag.size());
}

auto Flags::Get(std::string flag, const char **args_begin, const char **args_end) -> std::string
{
    flag += kDefPrefix;
    for(; args_begin != args_end; args_begin++)
    {
        std::string value = Get(*args_begin, flag);
        if(value != "")
            return value;
    }

    return "";
}

auto Flags::Get(std::string flag, const char **&pos, const char **args_begin, const char **args_end) -> std::string
{
    flag += kDefPrefix;
    for(; args_begin != args_end; args_begin++)
    {
        std::string value = Get(*args_begin, flag);
        if(value != "")
        {
            pos = args_begin;
            return value;
        }
    }

    pos = nullptr;
    return "";
}

void Flags::Parse()
{
    for(const char **it = begin_; it < end_; it++)
    {
        const std::string str(*it);
        if(StringStartsWith(prefix_, str))
        {
            std::size_t sep_pos = str.find(separator_);
            if(sep_pos == std::string::npos)
            {
                flags_[str];
                continue;
            }

            flags_[str] = str.substr(sep_pos);
        }
    }
}
