#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"
#include <crails/utils/string.hpp>

class I18nPlugin : public CommandIndex
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

  I18nPlugin();

  std::string_view description() const override { return "translation helpers"; }

  static bool build(const ProjectConfiguration&);
};
