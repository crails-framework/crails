#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"
#include <crails/utils/string.hpp>

class RedisPlugin : public CommandIndex
{
public:
  struct RedisInstaller : public ::Command
  {
    int  run() override;
  };

  struct RedisDisabler : public ::Command
  {
    int run() override;
  };

  RedisPlugin();

  std::string_view description() const override { return "provides redis database support"; }
};

