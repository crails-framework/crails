#pragma once
#include "file_editor.hpp"
#include "toolchain_editor.hpp"

class Build2Editor : public ToolchainEditor
{
public:
  Build2Editor(const ProjectConfiguration& configuration);

  bool load_file() override;
  void save_file() override;
  void update_plugins() override;
  void add_definitions(const std::set<std::string>&) override;
  void add_dependency(const std::string& name, const std::string& category) override;
  void remove_definitions(const std::set<std::string>& definitions) override;
  void set_config_variable(const std::string& name, const std::string& type, const std::string& value);
private:
  std::string repository_entry_for(const std::string& plugin) const;
  void require_repository_for(const std::string& plugin_);
  void require_depends_for(const std::string& library, const std::string& version = "");
  void import_library(const std::string& library, CrailsFileEditor&);
  void append_definition_export(const std::string& definition);
  void remove_definition_export(const std::string& definition);

  CrailsFileEditor buildfile, manifest, repositories, root;
  CrailsFileEditor test_buildfile;
};
