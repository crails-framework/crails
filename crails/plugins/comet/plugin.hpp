#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"
#include <crails/utils/string.hpp>

class CometPlugin : public CommandIndex
{
public:
  std::string_view description() const override { return "integrates the comet framework using a C++ to JavaScript and WebAssembly compiler"; }

  struct CometInstaller : public ::Command
  {
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("client-path,c", boost::program_options::value<std::string>(), "initialize the comet project in a custom path")
        ("cheerp-path,p", boost::program_options::value<std::string>(), "cheerp installation path");
    }
  };

  CometPlugin();

  static std::string find_comet_command(const ProjectConfiguration&);
  static std::string asset_exclusion_pattern(const ProjectConfiguration&);
  static bool build(const ProjectConfiguration&, bool verbose);
};
