#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"

class SigninPlugin : public CommandIndex
{
public:
  struct Installer : public ::Command
  {
    int  run() override;
  };

  struct Disabler : public ::Command
  {
    int run() override;
  };

  SigninPlugin();

  std::string_view description() const override { return "adds user session helpers and password encryption"; }
};


