#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"

class SentryPlugin : public CommandIndex
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

  SentryPlugin();

  std::string_view description() const override { return "logs exceptions to a Sentry server"; }
};
