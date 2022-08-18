#pragma once
#include <map>
#include <list>
#include <string>

class ProjectConfiguration
{
  std::map<std::string, std::string> variables;
public:
  ProjectConfiguration();

  static void        create();
  static std::string project_directory();
  static void        move_to_project_directory();

  void initialize();
  void save();

  std::string version() const;
  void        version(const std::string&);

  std::string toolchain() const;
  void        toolchain(const std::string&);

  std::list<std::string> modules() const;
  void                   modules(const std::list<std::string>&);
  void                   add_module(const std::string&);
  void                   remove_module(const std::string&);
  bool                   has_module(const std::string&) const;

  std::list<std::string> renderers() const;
  void                   renderers(const std::list<std::string>&);
  void                   add_renderer(const std::string&);
  void                   remove_renderer(const std::string&);

  std::list<std::string> asset_roots() const;
  void                   asset_roots(const std::list<std::string>&);

  std::string crails_bin_path() const;
  std::string application_build_path() const;
};
