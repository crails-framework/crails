#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"
#include <crails/utils/string.hpp>

class CometPlugin : public CommandIndex
{
public:
  struct CometInstaller : public ::Command
  {
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("comet-path,c", boost::program_options::value<std::string>(), "initialize the comet project in a custom path")
        ("cheerp-path,p", boost::program_options::value<std::string>(), "cheerp installation path");
    }
  };

  CometPlugin();

  std::string_view description() const override { return "the comet plugins integrates a C++ to JavaScript transpiler"; }

  static std::string find_comet_html_generator();
  static bool generate_comet_views(const ProjectConfiguration&);
};
