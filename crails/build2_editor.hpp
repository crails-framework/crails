#pragma once
#include "file_editor.hpp"
#include "project_configuration.hpp"
#include <set>

class Build2Editor
{
public:
  Build2Editor(const ProjectConfiguration& configuration);

  bool load_file();
  void save_file();
  void update_plugins();
  void add_definitions(const std::set<std::string>&);
private:
  const ProjectConfiguration& configuration;
  CrailsFileEditor buildfile, manifest, repositories;
};
