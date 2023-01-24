#pragma once
#include <crails/cli/conventions.hpp>

class ProjectConfiguration;

std::string app_renderer_filename(const ProjectConfiguration& configuration, const std::string& renderer);
std::string app_renderer_classname(const ProjectConfiguration& configuration, const std::string& renderer);


