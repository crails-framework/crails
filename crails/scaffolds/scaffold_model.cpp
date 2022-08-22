#include <crails/utils/string.hpp>
#include "scaffold_model.hpp"

using namespace std;

map<string, string> ScaffoldModel::properties_option(const boost::program_options::variables_map& options)
{
  map<string, string> properties;

  if (options.count("property"))
  {
    for (const string& property_decl : options["property"].as<vector<string>>())
    {
      auto parts = Crails::split(property_decl, '/');
      properties.emplace(*parts.rbegin(), *parts.begin());
    }
  }
  return properties;
}
