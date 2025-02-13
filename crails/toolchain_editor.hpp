#pragma once
#include <set>
#include <string>

class ProjectConfiguration;

class ToolchainEditor
{
public:
  ToolchainEditor(const ProjectConfiguration& configuration)
    : configuration(configuration)
  {
  }

  virtual bool load_file() = 0;
  virtual void save_file() = 0;
  virtual void update_plugins() {}
  virtual void add_dependency(const std::string& name, const std::string& category = "dependencies") {};
  virtual void add_definitions(const std::set<std::string>&) {}
  virtual void remove_definitions(const std::set<std::string>&) {}
  virtual void add_task(const std::string& name) {}
  virtual void add_module(const std::string& name) {}
  virtual void remove_module(const std::string& name) {}
protected:
  const ProjectConfiguration& configuration;
};
