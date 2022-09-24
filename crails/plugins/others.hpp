#pragma once
#include "../command.hpp"
#include "../cmake_module_updater.hpp"
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
    CrailsPackageListEditor editor;

    if (options.count("add"))
    {
      for (const std::string& plugin : options["add"].as<std::vector<std::string>>())
        configuration.add_plugin(plugin);
    }
    if (options.count("remove"))
    {
      for (const std::string& plugin : options["remove"].as<std::vector<std::string>>())
        configuration.remove_plugin(plugin);
    }
    if (editor.update_plugins_list(configuration.plugins(), configuration.version()))
    {
      editor.save_file();
      configuration.save();
      return 0;
    }
    if (options.count("list"))
    {
      for (const std::string& plugin : configuration.plugins())
        std::cout << plugin << std::endl;
    }
    return -1;
  }
};
