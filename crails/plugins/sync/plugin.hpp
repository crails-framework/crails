#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"

class SyncPlugin : public CommandIndex
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

  SyncPlugin();

  std::string_view description() const override { return "adds client-server synchronization tools based on WebSockets"; }
};

