#pragma once
#include <map>
#include <list>
#include <string>

class ProjectConfiguration
{
  std::map<std::string, std::string> variables;
public:
  ProjectConfiguration();

  static std::string project_directory();
  static void        move_to_project_directory();

  void initialize();
  void save();

  std::string variable(const std::string& name) const { return variables.find(name) != variables.end() ? variables.at(name) : ""; }
  void        variable(const std::string& name, const std::string& value) { variables[name] = value; }

  std::string version() const;
  void        version(const std::string&);

  std::string toolchain() const;
  void        toolchain(const std::string&);

  std::list<std::string> plugins() const;
  void                   plugins(const std::list<std::string>&);
  void                   add_plugin(const std::string&);
  void                   remove_plugin(const std::string&);
  bool                   has_plugin(const std::string&) const;

  std::list<std::string> modules() const;
  void                   add_module(const std::string&);
  void                   remove_module(const std::string&);

  std::list<std::string> renderers() const;
  void                   renderers(const std::list<std::string>&);
  void                   add_renderer(const std::string&);
  void                   remove_renderer(const std::string&);

  std::list<std::string> asset_roots() const;
  void                   asset_roots(const std::list<std::string>&);

  std::string crails_bin_path() const;
  std::string application_build_path() const;
};
