#include "FlagParser.hpp"

#include <string>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

#include "StringUtils.hpp"


auto Flags::Count(const std::string &flag, const char **args_begin, const char **args_end) -> std::size_t
{
    std::size_t count = 0;

    for(; args_begin != args_end; args_begin++)
        count += StringStartsWith(flag, std::string(*args_begin));

    return count;
}

auto Flags::Contains(const std::string &flag, const char **args_begin, const char **args_end) -> const char **
{
    for(; args_begin != args_end; args_begin++)
    {
        if(StringStartsWith(flag, std::string(*args_begin)))
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

    return str.substr(flag.size());
}

auto Flags::ParseFlag(const char **flag_begin, const char **possible_flag_end, std::size_t *strings_used, const std::string &delim) -> MapType::value_type
{
    enum
    {
        kReadingKey,
        kReadingDelimetr,
        kValueCrossroad,
        kQuotedValue,
        kInternalQuotationMark,
        kEndOfQuote,
        kUnquotedValue
    };

    if(delim.empty())
        return std::make_pair(flag_begin[0], "");

    // always at least one is used
    if(strings_used != nullptr)
        *strings_used = 1;

    int state = kReadingKey;
    int in_state = 0;
    std::size_t flag_size = strlen(*flag_begin);
    std::string key;
    std::string value;

    std::size_t i = 0;
    while(true)
    {
        switch(state)
        {
        case kReadingKey:
            if((*flag_begin)[i] == '\0')
            {
                if(in_state == 0)
                {
                    // Parse error
                    std::cout << "Flag parsing error: expected key, got null terminator in flag " << *flag_begin << '\n';
                    return MapType::value_type();
                }

                return std::make_pair(flag_begin[0], "");
            }

            if((*flag_begin)[i] == delim[0])
            {
                if(in_state == 0)
                {
                    // Parse error
                    std::cout << "Flag parsing error: expected key name before value delimetr in flag" << *flag_begin << '\n';
                    return MapType::value_type();
                }

                key = std::string((*flag_begin), in_state);
                
                in_state = 0;
                state = kReadingDelimetr;
                i--;
                break;
            }

            in_state++;
            break;
        case kReadingDelimetr:
            if((*flag_begin)[i] == '\0')
            {
                // Parse error
                std::cout << "Flag parsing error: expected delimetr, got null terminator in flag " << *flag_begin << '\n';
                return MapType::value_type();
            }

            if(static_cast<std::size_t>(in_state) == delim.size())
            {
                in_state = 0;
                state = kValueCrossroad;
                i--;
                break;
            }

            if((*flag_begin)[i] != delim[in_state])
            {
                // Parse error
                std::cout << "Flag parsing error: invalid delimetr. Expected " << in_state << ' ' << delim << " got " << std::string(&(*flag_begin)[i - in_state], in_state + 1) << " in flag " << *flag_begin << '\n';
                return MapType::value_type();
            }

            in_state++;
            break;
        case kValueCrossroad:
            if((*flag_begin)[i] == '"')
            {
                state = kQuotedValue;
                break;
            }
            else if((*flag_begin)[i] == '\0') // it stays under, because other variants are more likely
            {
                // Parse error
                std::cout << "Flag parsing error: expected value, got null terminator in flag" << *flag_begin << '\n';
                return MapType::value_type();
            }

            state = kUnquotedValue;
            i--; // unquoted value needs to know about current char
            break;
        case kQuotedValue:
            switch((*flag_begin)[i])
            {
            case '\0':
                value += std::string((*flag_begin) + flag_size - in_state, in_state) + " ";

                flag_begin++;
                if(flag_begin >= possible_flag_end)
                {
                    // Parse error
                    std::cout << "Flag parsing error: expected quotation mark, got null terminator in flag" << *(flag_begin - 1) << '\n';
                    return MapType::value_type();
                }

                flag_size = strlen(*flag_begin);
                if(strings_used != nullptr)
                    *strings_used += 1;

                i = 0;
                in_state = 0;
                continue;
            case '\\':
                value += std::string((*flag_begin) + i - in_state, in_state);
                in_state = 0;
                state = kInternalQuotationMark;
                break;
            case '"':
                value += std::string((*flag_begin) + flag_size - in_state - 1, in_state); // - 1 to account null terminator, we expect it to be next, if not parsing error is printed and empty string returned
                state = kEndOfQuote;
                break;
            default:
                break;
            }

            in_state++;
            break;
        case kInternalQuotationMark:
            if((*flag_begin)[i] == '\0')
            {
                value += std::string((*flag_begin) + flag_size - in_state, in_state) + " ";

                flag_begin++;
                if(flag_begin >= possible_flag_end)
                {
                    // Parse error
                    std::cout << "Flag parsing error: expected quotation mark, got null terminator in flag " << *(flag_begin - 1) << '\n';
                    return MapType::value_type();
                }

                flag_size = strlen(*flag_begin);
                if(strings_used != nullptr)
                    *strings_used += 1;

                i = 0;
                continue;
            }

            state = kQuotedValue;
            break;
        case kEndOfQuote:
            if((*flag_begin)[i] == '\0')
                return std::make_pair(key, value);
            
            // Parse error
            std::cout << "Flag parsing error: expected null terminator after quotation mark, got " << (*flag_begin)[i] << " in flag " << *flag_begin << '\n';
            return MapType::value_type();
        case kUnquotedValue:
            return std::make_pair(key, std::string(&(*flag_begin)[i]));
        }

        i++;
    }
}

auto Flags::Get(const std::string &flag, const char **args_begin, const char **args_end) -> std::string
{
    std::size_t strings_used = 0;
    for(; args_begin != args_end; args_begin++)
    {
        const auto &key_and_value = ParseFlag(args_begin, args_end, &strings_used);
        if(key_and_value.first == flag)
            return key_and_value.second;
        
        args_begin += strings_used;
    }

    return "";
}

void Flags::Parse()
{
    std::size_t strings_used = 0;
    for(const char **str = begin_; str != end_;)
    {
        flags_.insert(ParseFlag(str, end_, &strings_used, delim_));
        str += strings_used;
    }
}