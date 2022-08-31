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
      ("verbose,v", "enable verbose mode");
  }

private:
  bool prebuild_renderers();
  bool generate_assets();
  bool generate_database();
};
