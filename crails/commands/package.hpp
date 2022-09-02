#pragma once
#include "../command.hpp"

class Package : public ::Command
{
public:
  std::string_view description() const override { return "Packages your crails application for deployment."; }
  int              run() override;
  void             options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("verbose,v", "enable verbose mode")
      ("mode,m",    "Debug or Release: defaults to the build-type value set in the .crails file")
      ("output,o",  "output file (defaults to application.tar.gz)");
  }

};
