#ifndef FLAG_PARSER_HPP
#define FLAG_PARSER_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>


// Check if string starts with certain prefix
// Doesn't perform bounds checking so if prefix.size() > string.size() behaviour is undefined
bool StringStartsWith(const std::string &prefix, const std::string &string);



// Class to parse flags
// Flag format must be next $(some_prefix)flag
// Also values being supported in format of $(some_prefix)flag$(some_separator)value
class Flags
{
public:
    // Parse flags from cmd with prefix "-" and separator "="
    inline Flags(const int argc, const char **argv) : begin_(argv), end_(argv + argc) { Parse(); }
    // Parse flags from cmd with custom prefix and separator
    inline Flags(const int argc, const char **argv, const std::string &flag_prefix, const std::string &flag_separator) : begin_(argv), end_(argv + argc), prefix_(flag_prefix), separator_(flag_separator) { Parse(); }

    // Returns count of flags
    inline auto Count(const std::string &flag) const -> int { return flags_.count(flag); }
    // Returns true if there is such flag
    inline auto Contains(const std::string &flag) const -> bool { return Count(flag) != 0; }
    // Get flag value
    // If flag doesn't contain a value or don't exists empty string is returned
    inline auto Get(const std::string &flag) const -> std::string { const auto it = flags_.find(flag); return (it == flags_.end() ? "" : it->second); }

    inline constexpr auto GetBegin() const -> const char** { return begin_; }
    inline constexpr auto GetEnd() const -> const char** { return end_; }

    auto GetArgsAsString(const std::string &delimetr = " ") const -> std::string;


    // Return count of flags in range of attributes
    static auto Count(const std::string &flag, const char **args_begin, const char **args_end) -> std::size_t;
    // Returns position of the flag argument if it is found in range of attributes
    static auto Contains(const std::string &flag, const char **args_begin, const char **args_end) -> const char**;
    // Return flag value from string
    static auto Get(const std::string &flag, const std::string &str) -> std::string;
    // Return flag value from first occurence in range of attributes
    static auto Get(std::string flag, const char **args_begin, const char **args_end) -> std::string;
    // Return flag value from first occurence in range of attributes with reference to position of that attribute
    static auto Get(std::string flag, const char **&pos, const char **args_begin, const char **args_end) -> std::string;

private:
    const char **begin_;
    const char **end_;

    static constexpr const char *kDefPrefix = "-";
    static constexpr const char *kDefSeparator = "=";

    const std::string prefix_ = kDefPrefix;
    const std::string separator_ = kDefSeparator;

    std::unordered_map<std::string, std::string> flags_;

    void Parse();
};

#endif // FLAG_PARSER_HPP