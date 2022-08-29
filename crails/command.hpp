#pragma once
#include <crails/cli/command.hpp>
#include "project_configuration.hpp"

class Command : public Crails::Command
{
public:
  virtual int run(int argc, char** argv) override
  {
    int result = Crails::Command::run(argc, argv);

    if (result == 0 && with_configuration)
      configuration.initialize();
    return result;
  }

protected:
  ProjectConfiguration configuration;
  bool with_configuration = true;
};
