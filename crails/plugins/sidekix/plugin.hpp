#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"

class SidekixPlugin : public CommandIndex
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

  struct AddTask : public ::Command
  {
    int run() override;

    std::string_view description() const override { return "scaffolds a new side-task"; }
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("name,n",   boost::program_options::value<std::string>(), "task's name");
    }
  };

  SidekixPlugin();

  std::string_view description() const override { return "runs background tasks in a separate process"; }
};



