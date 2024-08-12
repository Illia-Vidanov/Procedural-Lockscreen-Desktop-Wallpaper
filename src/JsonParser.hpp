#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <string>
#include <memory>

#include "nlohmann/json.hpp" // better to not forward declarate type aliases and templeted classes

#include "ImageData.hpp"


class Flags;

auto InitializeJson(const Flags &flags) -> std::shared_ptr<nlohmann::json>;
auto GetImageDataFromJson(std::shared_ptr<nlohmann::json> json) -> ImageData;
auto GetLastScriptFromJson(std::shared_ptr<nlohmann::json> json) -> std::string;
auto GetJsonFile(const std::string &json_path) -> std::shared_ptr<nlohmann::json>;
void WriteJsonToFile(std::shared_ptr<nlohmann::json> json, const std::string &path);

#endif // JSON_PARSER_HPP