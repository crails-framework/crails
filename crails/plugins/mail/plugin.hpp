#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"
#include <crails/utils/string.hpp>

class MailPlugin : public CommandIndex
{
public:
  static const std::vector<std::string> supported_backends;

  static bool check_backends_validity(const std::list<std::string>& backends);

  struct Installer : public ::Command
  {
    int  run() override;
  };

  struct Disabler : public ::Command
  {
    int run() override;
  };

  MailPlugin();

  std::string_view description() const override { return "adds an SMTP server and tools to send mail via your application controllers"; }
};
