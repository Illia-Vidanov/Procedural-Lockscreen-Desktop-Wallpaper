#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <filesystem>
#include <string>


inline auto AddCurrentPathToString(const std::string &str) -> std::string { return std::filesystem::current_path().generic_u8string() + "/" + str; }
inline auto AddCurrentPathToString(const std::wstring &str) -> std::wstring { return std::filesystem::current_path().generic_wstring() + L"/" + str; }

#endif