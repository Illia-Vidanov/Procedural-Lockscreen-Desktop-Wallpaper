#ifndef GENERATE_IMAGE_HPP
#define GENERATE_IMAGE_HPP

#include "ImageData.hpp"

#include <string>
#include <vector>


class Flags;
class ScriptManager;

void GenerateImage(const Flags &flags, const ScriptManager &script_manager);
void GenerateBlanckImage(const ImageData &image_data);
void ProcessScript(const ImageData &image_data, const std::string &script);
void ProcessLua(const ImageData &image_data, const std::string &script);
void ProcessExe(const ImageData &image_data, const std::string &script);

#endif // GENERATE_IMAGE_HPP