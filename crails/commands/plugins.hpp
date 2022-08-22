#pragma once
#include "../command_index.hpp"

class PluginManager : public CommandIndex
{
public:
  PluginManager();
  std::string_view description() const override { return "manages plugins and their custom settings"; }
};
