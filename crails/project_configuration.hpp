#pragma once
#include <crails/cli/project_variables.hpp>
#include <list>

class ProjectConfiguration : public Crails::ProjectVariables
{
public:
  ProjectConfiguration();

  static std::string project_directory();
  static void        move_to_project_directory();

  std::string            project_name() const;

  std::string            version() const;
  void                   version(const std::string&);

  std::string            toolchain() const;
  void                   toolchain(const std::string&);

  std::string            build_type() const;
  void                   build_type(const std::string&);

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

  static std::string crails_bin_path();
  std::string application_build_path() const;
};
