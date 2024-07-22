#ifndef FLAG_PARSER_HPP
#define FLAG_PARSER_HPP

#include <string>

// Return flag info from string
// Example flag = "-f" and str = "-fA123"
// "A123" returned
// Note: flag must be at the start of the attribute. "A12-f3" won't work
std::string GetFlag(const std::string &str, const std::string &flag);
// Return flag info from range of attributes
// Example flag = "-f" somewhere between 'args_begin' and 'args_end' exists "-fA123"
// "A123" returned
// Note: flag must be at the start of the attribute. "A12-f3" won't work
std::string GetFlag(const char **args_begin, const char **args_end, const std::string &flag);
// Return flag info from range of attributes with reference to position of that attribute
// Example flag = "-f" somewhere between 'args_begin' and 'args_end' exists "-fA123"
// "A123" returned
// Note: flag must be at the start of the attribute. "A12-f3" won't work
std::string GetFlag(const char **args_begin, const char **args_end, const std::string &flag, const char ***pos);

// Returns position th flag if flag found in range of attributes
const char **FlagExists(const char **args_begin, const char **args_end, const std::string &flag);

#endif // FLAG_PARSER_HPP