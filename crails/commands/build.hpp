#pragma once
#include "../command.hpp"

class BuildManager : public ::Command
{
public:
  std::string_view description() const override { return "Builds a Crails server."; }
  int              run() override;
  void             options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("verbose,v", "enable verbose mode")
      ("mode,m", boost::program_options::value<std::string>(), "Debug or Release: defaults to the build-type value set in the .crails file")
      ("clean,c",   "cleanup before building");
  }

private:
  bool prebuild_renderers();
  bool generate_assets();
  bool generate_database();
};
