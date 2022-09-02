#include "plugin.hpp"
#include <crails/utils/string.hpp>
#include <boost/process.hpp>
#include <crails/cli/process.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

static const string generator_key = "metarecord-generators";

MetarecordPlugin::MetarecordPlugin()
{
  add_command("install",    []() { return std::make_shared<MetarecordInstaller>(); });
  add_command("generators", []() { return std::make_shared<MetarecordGenerators>(); });
}

void MetarecordPlugin::MetarecordInstaller::options_description(boost::program_options::options_description& options) const
{
  options.add_options()
    ("generator,g", boost::program_options::value<vector<string>>()->multitoken(), "use a generator");
}

static void cannot_find_ruby()       { cerr << "[metarecord] cannot find ruby. Maybe it is not installed ?" << endl; }
static void cannot_find_metarecord() { cerr << "[metarecord] cannot find metarecord-make. Run `gem install metarecord`" << endl; }

static bool run_ruby_command(const string& command)
{
  string ruby = Crails::which("ruby");

  if (ruby.length() > 0)
    return Crails::run_command(ruby + ' ' + command);
  else
    cannot_find_ruby();
  return false;
}

static bool list_available_generators()
{
  string metarecord = Crails::which("metarecord-make");

  if (metarecord.length())
    return run_ruby_command(metarecord + " --list-generators");
  else
    cannot_find_metarecord();
  return false;
}

int MetarecordPlugin::MetarecordInstaller::run()
{
  if (!options.count("generator"))
  {
    cout << "please specify at least one generator:" << endl;
    list_available_generators();
    return -1;
  }
  configuration.add_plugin("metarecord");
  configuration.variable(generator_key, Crails::join(options["generator"].as<vector<string>>(), ','));
  configuration.save();
  return 0;
}

void MetarecordPlugin::MetarecordGenerators::options_description(boost::program_options::options_description& options) const
{
  options.add_options()
    ("list,l",   "lists available generators")
    ("add,a",    boost::program_options::value<vector<string>>()->multitoken(), "add a generator")
    ("remove,r", boost::program_options::value<vector<string>>()->multitoken(), "remove a generator");
}

int MetarecordPlugin::MetarecordGenerators::run()
{
  list<string> generators = Crails::split(configuration.variable(generator_key), ',');

  if (options.count("list"))
    return list_available_generators() ? 0 : -1;
  if (!options.count("add") && !options.count("remove"))
  {
    for (const string& generator : generators)
      cout << "- " << generator << endl;
  }
  if (options.count("add"))
  {
    auto new_modules = options["add"].as<vector<string>>();
    std::copy(new_modules.begin(), new_modules.end(), std::back_inserter(generators));
  }
  if (options.count("remove"))
  {
    auto removed_modules = options["remove"].as<vector<string>>();
    std::remove_if(generators.begin(), generators.end(), [&removed_modules](const string& mod)
    { return std::find(removed_modules.begin(), removed_modules.end(), mod) != removed_modules.end(); });
  }
  configuration.variable(generator_key, Crails::join(generators, ','));
  configuration.save();
  return 0;
}

static list<string> get_input_paths(const ProjectConfiguration& configuration)
{
  list<string> paths{"app/data"};

  for (const string& mod : configuration.modules())
    paths.push_back("modules/" + mod + "/data");
  return paths;
}

bool MetarecordPlugin::build(const ProjectConfiguration& configuration, bool verbose)
{
  string bin = Crails::which("metarecord-make");
  stringstream command;

  if (bin.length() > 0)
  {
    command << bin
      << " -o lib"
      << " -i " << Crails::join(get_input_paths(configuration), ',')
      << " -g " << configuration.variable("metarecord-generators")
      << " -z .tmp";
    if (verbose)
      cout << "+ " << command.str() << endl;
    return run_ruby_command(command.str());
  }
  else
    cannot_find_metarecord();
  return false;
}
