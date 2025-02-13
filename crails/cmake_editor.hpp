#pragma once
#include "toolchain_editor.hpp"
#include "file_editor.hpp"

class CmakeEditor : public ToolchainEditor
{
public:
  CmakeEditor(const ProjectConfiguration&);

  bool load_file() override { return root.load_file(); }
  void save_file() override { root.save_file(); }

  std::string plugins_config_line() const;
  void update_plugins() override;
  void add_dependency(const std::string& name, const std::string& category) override;
  void add_definitions(const std::set<std::string>&) override;
  void remove_definitions(const std::set<std::string>&) override;
  void add_task(const std::string& name) override;
  void add_module(const std::string& name) override;
  void remove_module(const std::string& name) override;

private:
  CMakeFileEditor root;
};
