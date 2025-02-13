#pragma once
#include "../command.hpp"
#include <iostream>

class OtherPlugins : public ::Command
{
public:
  std::string_view description() const override { return "link/unlink your application to crails libraries"; }

  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("list,l",   "lists the plugins that are currently active")
      ("add,a",    boost::program_options::value<std::vector<std::string>>()->multitoken(), "plugins to add")
      ("remove,r", boost::program_options::value<std::vector<std::string>>()->multitoken(), "plugins to remove");
  }

  int run() override
  {
    bool with_changes = false;

    if (options.count("add"))
    {
      with_changes = true;
      for (const std::string& plugin : options["add"].as<std::vector<std::string>>())
        configuration.add_plugin(plugin);
    }
    if (options.count("remove"))
    {
      with_changes = true;
      for (const std::string& plugin : options["remove"].as<std::vector<std::string>>())
        configuration.remove_plugin(plugin);
    }
    if (with_changes)
    {
      configuration.update_plugins();
    }
    if (options.count("list"))
    {
      for (const std::string& plugin : configuration.plugins())
        std::cout << plugin << std::endl;
    }
    return -1;
  }
};
