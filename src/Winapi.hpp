#ifndef WIN_API_HPP
#define WIN_API_HPP

// Start process specified by path, with arguments and wait till it finished, exit code is returned. All arguments must be passed in utf8 encoding
auto ExecuteProgram(const char *program_path, char *args) -> int;
void TerminateAdmin();
void GetScreenResolution(int &width, int &height);

auto Utf8ToWideChar(const char *str) -> wchar_t*;
auto WideCharToUtf8(const wchar_t *str) -> char*;
auto ConvertArgsToUtf8(int argc, const char **argv) -> const char**;

#endif // WIN_API_HPP