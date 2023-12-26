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

static void cannot_find_metarecord() { cerr << "[metarecord] cannot find metarecord-make. Run `gem install metarecord`" << endl; }

static bool list_available_generators()
{
  string metarecord = Crails::which("metarecord-make");

  if (metarecord.length())
    return Crails::run_command(metarecord + " --list-generators");
  else
    cannot_find_metarecord();
  return false;
}

static list<string> get_generators(const ProjectConfiguration& configuration)
{
  list<string> generators;

  if (configuration.has_plugin("odb"))
  {
    if (configuration.has_plugin("comet"))
    {
      generators.push_back("comet/data");
      generators.push_back("comet/edit");
    }
    else
    {
      generators.push_back("comet/data");
      generators.push_back("crails/edit");
    }
    generators.push_back("crails/query");
    generators.push_back("crails/destroy");
  }
  for (const string& generator : Crails::split(configuration.variable(generator_key), ','))
  {
    if (find(generators.begin(), generators.end(), generator) == generators.end())
      generators.push_back(generator);
  }
  return generators;
}

bool MetarecordPlugin::has_view_generator(const ProjectConfiguration& configuration)
{
  if (configuration.has_plugin("metarecord"))
  {
    auto generators = get_generators(configuration);

    return std::find(generators.begin(), generators.end(), "crails/view") != generators.end();
  }
  return false;
}

int MetarecordPlugin::MetarecordInstaller::run()
{
  auto generators = get_generators(configuration);

  if (!options.count("generator") && generators.size() == 0)
  {
    cout << "please specify at least one generator:" << endl;
    list_available_generators();
    return -1;
  }
  configuration.add_plugin("metarecord");
  configuration.variable(generator_key, Crails::join(options["generator"].as<vector<string>>(), ','));
  configuration.save();
  if (Crails::which("ruby").length() == 0)
    cout << "(i) Ruby was not found on your system. It is required to run metarecord generators." << endl;
  cout << "(i) Run `gem install metarecord` to install the metarecord generators on your system." << endl;
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
  list<string> generators = get_generators(configuration);

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
      << " -g " << Crails::join(get_generators(configuration), ',')
      << " -z .tmp";
    if (verbose)
    {
      command << " -v";
      cout << "+ " << command.str() << endl;
    }
    return Crails::run_command(command.str());
  }
  else
    cannot_find_metarecord();
  return false;
}
