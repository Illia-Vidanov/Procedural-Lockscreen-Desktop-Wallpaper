#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

constexpr inline int kImageDepth = 3;
constexpr inline int kImageStrideMultipleOf = 8;

constexpr static const wchar_t *kPersonalizationFolder = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP";
constexpr static const wchar_t *kImagePathKey = L"LockScreenImagePath";

constexpr inline const char *kWidthKey = "width";
constexpr inline const char *kHeightKey = "height";
constexpr inline const char *kPathKey = "path";
constexpr inline const char *kLastScriptKey = "last_script";

constexpr inline const char *kGenerateName = "Generate.exe";
constexpr inline const char *kJsonName = "props.json";
constexpr inline const char *kImageName = "Image.png";
constexpr inline const wchar_t *kImageNameW = L"Image.png";
constexpr inline const char *kScriptsFolderName = "scripts";

constexpr inline const char *kDefTaskName = "Generate Wallpaper";
constexpr inline const char *kDefPowershellPath = "C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe";

constexpr inline const char *kCleanFlag = "-clean";
constexpr inline const char *kCleanAllFlag = "-clean-all";
constexpr inline const char *kCleanRegFlag = "-clean-registry";
constexpr inline const char *kCleanFilesFlag = "-clean-files";
constexpr inline const char *kCleanTaskFlag = "-clean-task";

constexpr inline const char *kHelpFlag = "-help";

constexpr inline const char *kSkipIntegrityFlag = "-skip-integrity";
constexpr inline const char *kSkipJsonFlag = "-skip-json";
constexpr inline const char *kSkipRegistryFlag = "-skip-registry";
constexpr inline const char *kSkipGenerateFlag = "-skip-generate";
constexpr inline const char *kSkipTaskFlag = "-skip-task";

constexpr inline const char *kUserNameFlag = "-user-name";
constexpr inline const char *kPowershellPathFlag = "-powershell-path";
constexpr inline const char *kTaskNameFlag = "-task-name";

#endif // CONSTANTS_HPP