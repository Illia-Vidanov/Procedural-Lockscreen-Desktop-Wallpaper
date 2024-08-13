#ifndef FLAG_PARSER_HPP
#define FLAG_PARSER_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "StringUtils.hpp"


// Class to parse flags
// Values being supported in format of flag$(some_delimetr)value
class Flags
{
public:
    using MapType = std::unordered_multimap<std::string, std::string>;
    using RangeType = std::pair<MapType::const_iterator, MapType::const_iterator>;

    // Parse flags from cmd with delimetr "="
    inline Flags(const int argc, const char **argv) : begin_(argv), end_(argv + argc) { Parse(); }
    // Parse flags from cmd with custom  delimetr
    inline Flags(const int argc, const char **argv, const std::string &flag_delim) : begin_(argv), end_(argv + argc), delim_(flag_delim.empty() ? kDefDelim : flag_delim) { Parse(); }

    // Returns count of flags
    inline auto Count(const std::string &flag) const -> int { return flags_.count(flag); }
    // Returns true if there is such flag
    inline auto Contains(const std::string &flag) const -> bool { return Count(flag) != 0; }
    // Get flag value, if multiple are present any of them can be returned
    // If flag doesn't contain a value or don't exists empty string is returned
    inline auto Get(const std::string &flag) const -> std::string { const auto it = flags_.find(flag); return (it == flags_.end() ? "" : it->second); }
    // Get range of flags with same key in a pair
    // If flag doesn't exist past the end element is returned for both iterators
    inline auto GetRange(const std::string &flag) const -> RangeType { return flags_.equal_range(flag); }

    // Get constant iterator to begining of map
    inline auto begin() const -> MapType::const_iterator { return flags_.cbegin(); }
    // Get constant iterator to end of map
    inline auto end() const -> MapType::const_iterator { return flags_.cend(); }

    auto GetArgsAsString(const std::string &delim = " ") const -> std::string { return ConcatStringArray(begin_, end_, delim); }


    // Return count of flags in range of attributes
    static auto Count(const std::string &flag, const char **args_begin, const char **args_end) -> std::size_t;
    // Returns position of the flag argument if it is found in range of attributes
    static auto Contains(const std::string &flag, const char **args_begin, const char **args_end) -> const char**;
    // Return flag value from string
    static auto Get(const std::string &flag, const std::string &str) -> std::string;
    // Return flag value from first occurence in range of attributes
    // If flag doesn't contain a value or doesn't exist empty string is returned
    static auto Get(const std::string &flag, const char **args_begin, const char **args_end) -> std::string;


    static inline constexpr const char *kDefDelim = "=";

private:
    const char **begin_;
    const char **end_;

    const std::string delim_ = kDefDelim;

    MapType flags_;

    static auto ParseFlag(const char **flag_begin, const char **possible_flag_end, std::size_t *strings_used = nullptr, const std::string &delim = kDefDelim) -> MapType::value_type;
    void Parse();
};

#endif // FLAG_PARSER_HPP