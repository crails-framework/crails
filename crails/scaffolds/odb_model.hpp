#pragma once
#include "scaffold_model.hpp"
#include "../file_renderer.hpp"
#include "../file_editor.hpp"
#include "../project_configuration.hpp"
#include <crails/utils/string.hpp>

class OdbModelScaffold : public ScaffoldModel
{
  FileRenderer renderer;
  std::map<std::string, std::string> properties;
  std::string classname;
  std::string path_name;
  std::string target_folder = "app/models";
public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("name,n",       boost::program_options::value<std::string>(), "classname")
      ("target,t",     boost::program_options::value<std::string>(), "target folder (defaults to `app/controllers`)")
      ("properties,p", boost::program_options::value<std::vector<std::string>>(), "properties, such as: -p std::string;name \"unsigned int;age\"");
  }

  int create(boost::program_options::variables_map& options) override
  {
    ProjectConfiguration configuration;

    configuration.initialize();
    if (!options.count("name"))
    {
      std::cerr << "Option --name required" << std::endl;
      return -1;
    }
    classname = options["name"].as<std::string>();
    path_name = Crails::underscore(classname);
    if (options.count("properties"))
    {
      for (const std::string& property_decl : options["properties"].as<std::vector<std::string>>())
      {
        auto parts = Crails::split(property_decl, ';');
        properties.emplace(*parts.rbegin(), *parts.begin());
      }
    }
    renderer.vars["classname"] = classname;
    renderer.vars["properties"] = &properties;
    renderer.vars["filename"] = path_name;
    renderer.vars["odb_at_once"] = configuration.variable("odb-at-once") == "1";
    renderer.generate_file(
      "scaffolds/odb_model.hpp",
      target_folder + '/' + path_name + ".hpp"
    );
    renderer.generate_file(
      "scaffolds/odb_model.cpp",
      target_folder + '/' + path_name + ".cpp"
    );
    return 0;
  }
};
