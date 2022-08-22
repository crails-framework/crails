#include "formats.hpp"
#include <crails/utils/string.hpp>

using namespace std;

void TemplateFormatsManager::options_description(boost::program_options::options_description& desc) const
{
  desc.add_options()
    ("add,a",    boost::program_options::value<string>(), "list of renderers to add (ex: html,json)")
    ("remove,r", boost::program_options::value<string>(), "list of renderers to remove");
}

int TemplateFormatsManager::run()
{
  if (!options.count("add") && !options.count("remove"))
  {
    cout << Crails::join(configuration.renderers(), ',') << endl;
    return 0;
  }
  if (options.count("add"))
  {
    if (!configuration.has_plugin("libcrails-templates"))
      configuration.add_plugin("libcrails-templates");
    for (const auto& entry : Crails::split(options["add"].as<string>(), ','))
    {
      configuration.add_renderer(entry);
      configuration.add_plugin("libcrails-" + entry + "-views");
    }
  }
  if (options.count("remove"))
  {
    for (const auto& entry : Crails::split(options["remove"].as<string>(), ','))
      configuration.remove_renderer(entry);
  }
  configuration.save();
  return 0;
}
