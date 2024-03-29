#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"
#include <crails/utils/string.hpp>

class MongodbModule : public CommandIndex
{
public:
  struct Installer : public ::Command
  {
    int run() override;
  };

  struct Disabler : public ::Command
  {
    int run() override;
  };

  MongodbModule();

  std::string_view description() const override { return "the mongodb plugin provides mongodb support through crails' database system"; }
};
