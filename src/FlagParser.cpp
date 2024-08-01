#include "FlagParser.hpp"

#include <string>
#include <cstring>
#include <unordered_map>
#include <unordered_set>


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
            const std::size_t sep_pos = str.find(separator_);
            if(sep_pos == std::string::npos)
            {
                flags_[str];
                continue;
            }
            // Yeah it gets worse but everything is working!
            std::size_t quote_pos = sep_pos + 1;
            if(str[quote_pos] == '"')
            {
                const std::size_t value_pos = quote_pos + 1;
                // if value is there (i.e. is not "-flag=\"") find quote position and create substr, else set that it isn't inside current string and create empty string
                std::string res(value_pos < str.size() ? (str.substr(value_pos, quote_pos = str.find('"', value_pos) - value_pos)) : (quote_pos = std::string::npos, ""));
                while(quote_pos > str.size()) // check if it is inside current string
                {
                    it++;
                    if(it >= end_)
                        break;
                    
                    quote_pos = std::strchr(*it, '"') - *it;
                    res += ' ';
                    res += std::string(*it).substr(0, quote_pos);
                }

                flags_[str.substr(0, sep_pos)] = res;
                continue; 
            }

            flags_[str.substr(0, sep_pos)] = str.substr(quote_pos); // quote_pos same as (sep_pos + 1) here
        }
    }
}
