#pragma once
#include "../command_index.hpp"

class ModuleManager : public CommandIndex
{
public:
  ModuleManager();
  std::string_view description() const override { return "manages modules and their custom settings"; }
};
