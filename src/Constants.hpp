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

constexpr inline const char *kClearFlag = "-clear";
constexpr inline const char *kClearAllFlag = "-clear-all";
constexpr inline const char *kClearRegistryFlag = "-clear-registry";
constexpr inline const char *kClearFilesFlag = "-clear-files";
constexpr inline const char *kClearTaskFlag = "-clear-task";

constexpr inline const char *kHelpFlag = "-help";

constexpr inline const char *kSkipIntegrityFlag = "-skip-integrity";
constexpr inline const char *kSkipJsonFlag = "-skip-json";
constexpr inline const char *kSkipRegistryFlag = "-skip-registry";
constexpr inline const char *kSkipGenerateFlag = "-skip-generate";
constexpr inline const char *kSkipTaskFlag = "-skip-task";

constexpr inline const char *kUserNameFlag = "-user-name";
constexpr inline const char *kPowershellPathFlag = "-powershell-path";
constexpr inline const char *kTaskNameFlag = "-task-name";

constexpr inline const char *kWidthFlag = "-width";
constexpr inline const char *kHeightFlag = "-height";

constexpr inline const char *kScriptDisableFlag = "-script-disable";
constexpr inline const char *kScriptEnableFlag = "-script-enable";
constexpr inline const char *kScriptDeleteFlag = "-script-delete";
constexpr inline const char *kScriptListFlag = "-script-list";
constexpr inline const char *kScriptDisableAllFlag = "-script-disable-all";
constexpr inline const char *kScriptEnableAllFlag = "-script-enable-all";
constexpr inline const char *kScriptDeleteAllFlag = "-script-delete-all";

#endif // CONSTANTS_HPP