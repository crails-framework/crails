#pragma once
#include "../command.hpp"
#include "../file_renderer.hpp"
#include <crails/template.hpp>

class New : public ::Command
{
  typedef std::pair<std::string, std::string> IncludeAndConstructor;
  typedef std::vector<IncludeAndConstructor> IncludeAndConstructors;

  FileRenderer             renderer;
  Crails::SharedVars&      vars;
  IncludeAndConstructors   project_renderers;
  IncludeAndConstructors   project_parsers;
  IncludeAndConstructors   project_handlers;
  std::string              project_name;
  std::string              build_system = "cmake";
  std::string              configuration_type = "full";
  std::string              session_store = "NoSessionStore";
  std::list<std::string>   formats, plugins;
public:
  New();

  std::string_view description() const override { return "Creates a new Crails application."; }
  void             options_description(boost::program_options::options_description& desc) const override;
  int              run() override;
private:
  void             generate_build2_files();
  void             generate_cmake_files();
  bool             generate_project_structure();
  bool             generate_file(std::string_view filepath) { return renderer.generate_file(filepath, filepath); }
  bool             generate_database(const std::string& backend);

  void             prepare_renderers();
  void             prepare_request_pipeline();
  bool             validate_options();
  void             use_actions(bool value) { vars["with_action"] = value; }
  void             use_cookies(bool value) { vars["with_cookies"] = value; }
  void             use_databases(bool value) { vars["with_databases"] = value; }

  std::string_view target() const { return (options.count("target") ? options["target"] : options["name"]).as<std::string>(); }
  bool             move_to_project_directory();
};
