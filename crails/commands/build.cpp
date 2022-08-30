#include "build.hpp"
#include <crails/utils/string.hpp>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <crails/utils/string.hpp>
#include "odb.hpp"
#include "../plugins/comet/plugin.hpp"
#include <iostream>

using namespace std;

bool crails_cmake_builder(ProjectConfiguration& configuration);
bool run_command(const string& command);

bool BuildManager::prebuild_renderers()
{
  for (const auto& renderer : configuration.renderers())
  {
    stringstream command;

    cout << "[renderers] generate " << renderer << "..." << endl;
    command << configuration.crails_bin_path() + "/crails"
      << " templates build -r " << renderer << " -i app/views "
      << " -t Crails::" << Crails::camelize(renderer) << "Template"
      << " -z crails/" << renderer << "_template.hpp"
      << " -p \\." << renderer << "$";
    return run_command(command.str());
  }
  return true;
}

bool BuildManager::generate_assets()
{
  if (boost::filesystem::exists(configuration.crails_bin_path() + "/crails-assets"))
  {
    std::stringstream command;

    cout << "[assets] generate assets..." << endl;
    command << configuration.crails_bin_path() + "/crails-assets"
      << " -o public"
      << " -i " << Crails::join(configuration.asset_roots(), ',');
    for (const string& module_ : configuration.modules())
      command << " -i " << module_ << ':' << "modules/" << module_ << "assets";
    return run_command(command.str());
  }
  return true;
}

static string model_input_dirs(ProjectConfiguration& configuration)
{
  stringstream model_input_dirs;

  model_input_dirs << "app/models";
  for (const string& module_ : configuration.modules())
    model_input_dirs << ",modules/" << module_ << "/models";
  return model_input_dirs.str();
}

bool BuildManager::generate_database()
{
  if (configuration.has_plugin("libcrails-odb"))
  {
    BuildOdb odb_builder;
    std::vector<std::string> argv_array{
      "--input-dirs", model_input_dirs(configuration),
      "--output-dir","lib/odb"
    };
    const char* argv[argv_array.size() + 1];

    std::cout << "[crails-odb] generate database queries and schema..." << std::endl;
    argv[argv_array.size()] = 0;
    for (int i = 0 ; i < argv_array.size() ; ++i)
      argv[i] = argv_array[i].c_str();
    odb_builder.initialize(argv_array.size(), argv);
    return odb_builder.run() == 0;
  }
  return true;
}

int BuildManager::run()
{
  if (!prebuild_renderers()) return 1;
  if (!generate_assets()) return 2;
  if (!generate_database()) return 3;
  if (configuration.has_plugin("comet") && !CometPlugin::build(configuration)) return 4;
  if (configuration.toolchain() == "cmake")
    return crails_cmake_builder(configuration) ? 0 : 5;
  else
    cerr << "Build command not supported for " << configuration.toolchain() << endl;
  return -1;
}
