#pragma once
#include <crails/cli/command.hpp>
#include "project_configuration.hpp"

class Command : public Crails::Command
{
public:
  virtual bool initialize(int argc, const char** argv) override
  {
    bool result = Crails::Command::initialize(argc, argv);

    if (result && with_configuration)
      configuration.initialize();
    return result;
  }

protected:
  ProjectConfiguration configuration;
  bool with_configuration = true;
};
