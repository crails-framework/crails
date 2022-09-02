#pragma once
#include <crails/cli/scaffold_model.hpp>
#include "../project_configuration.hpp"
#include "../file_renderer.hpp"

class ModuleScaffold : public ScaffoldModel
{
  FileRenderer renderer;
  boost::filesystem::path path;
  std::string name;
  std::string define;
  std::string namespace_name;
public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("name,n",   boost::program_options::value<std::string>(), "module name");
  }

  int create(boost::program_options::variables_map& options) override;
  void update_router();
  void update_cmake(ProjectConfiguration& configuration);
};
