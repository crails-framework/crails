#include "build.hpp"
#include <crails/utils/string.hpp>
#include <crails/cli/process.hpp>
#include <crails/cli/notifier.hpp>
#include <crails/cli/conventions.hpp>
#include <crails/read_file.hpp>
#include <crails/utils/join.hpp>
#include <boost/process.hpp>
#include "odb.hpp"
#include "../plugins/comet/plugin.hpp"
#include "../plugins/metarecord/plugin.hpp"
#include "../plugins/i18n/plugin.hpp"
#include "../templates/app_renderer_name.hpp"
#include <filesystem>
#include <iostream>

using namespace std;

bool crails_cmake_builder(const ProjectConfiguration& configuration, const string& mode, const string& cxx_flags, bool verbose, bool clean);
bool crails_build2_builder(const ProjectConfiguration& configuration, bool verbose, bool clean);

bool BuildManager::prebuild_renderers()
{
  for (const auto& renderer : configuration.renderers())
  {
    stringstream command;
    vector<string> inputs{"app/views"};

    if (MetarecordPlugin::has_view_generator(configuration))
    {
      inputs.push_back("app/autogen/app");
      inputs.push_back("app/autogen/modules"); // TODO should module view really be part of app/autogen ?
    }
    for (const string& module_ : configuration.modules())
      inputs.push_back("modules/" + module_ + "/views");
    cout << "[renderers] generate " << renderer << "..." << endl;
    command << configuration.crails_bin_path() + "/crails"
      << " templates build -r " << renderer
      << " -i " << Crails::join(inputs, ',')
      << " -t Crails::" << Crails::camelize(renderer) << "Template"
      << " -z crails/" << renderer << "_template.hpp"
      << " -n " << app_renderer_classname(configuration, renderer)
      << " -p \\." << renderer << "$";
    if (renderer == "json")
      command << " -m raw -s stream";
    if (options.count("verbose"))
    {
      command << " -v";
      cout << "+ " << command.str() << endl;
    }
    if (!Crails::run_command(command.str()))
      return false;
  }
  return true;
}

bool BuildManager::generate_assets()
{
  if (filesystem::exists(configuration.crails_bin_path() + "/crails-assets"))
  {
    std::stringstream command;

    cout << "[assets] generate assets..." << endl;
    command << configuration.crails_bin_path() + "/crails-assets"
      << " -o public"
      << " -i " << Crails::join(configuration.asset_roots(), ' ');
    for (const string& module_ : configuration.modules())
      command << ' ' << module_ << ':' << "modules/" << module_ << "/assets";
    if (options.count("verbose"))
      cout << "+ " << command.str() << endl;
    return Crails::run_command(command.str());
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

struct ArgvArray
{
  const char** data;
  ArgvArray(const std::vector<std::string>& argv_array) { data = new const char*[argv_array.size() + 1]; }
  ~ArgvArray() { delete[] data; }
};

bool BuildManager::generate_database()
{
  if (configuration.has_plugin("libcrails-odb"))
  {
    BuildOdb odb_builder;
    std::vector<std::string> argv_array{
      "--input-dirs", model_input_dirs(configuration),
      "--output-dir","app/autogen/odb"
    };
    if (options.count("verbose")) argv_array.push_back("--verbose");
    ArgvArray argv(argv_array);

    std::cout << "[crails-odb] generate database queries and schema..." << std::endl;
    argv.data[argv_array.size()] = 0;
    for (int i = 0 ; i < argv_array.size() ; ++i)
      argv.data[i] = argv_array[i].c_str();
    if (options.count("verbose"))
      cout << "+ crails plugins odb build " << Crails::join(argv_array) << endl;
    odb_builder.initialize(argv_array.size(), argv.data);
    switch (odb_builder.run())
    {
    default:
      return false;
    case -2:
      cerr << "[crails-odb] Skipping odb compilation. Your models and database schema won't be updated." << endl;
    case 0:
      break;
    }
  }
  return true;
}

static void restart_server()
{
  std::string pid_string;

  if (Crails::read_file("crails.pid", pid_string))
  {
    boost::process::child kill("kill -USR2 " + pid_string);

    kill.wait();
  }
}

int BuildManager::run()
{
  bool verbose = options.count("verbose");
  bool clean = options.count("clean");
  int result = -1;
  string mode = configuration.variable("build-type");
  string cxx_flags;

  if (options.count("defines"))
  {
    for (const string& flag : options["defines"].as<vector<string>>())
      cxx_flags += "-D" + flag + ' ';
  }
  if (options.count("mode"))
    mode = options["mode"].as<string>();
  if (configuration.has_plugin("libcrails-i18n") && !I18nPlugin::build(configuration)) return 12;
  if (configuration.has_plugin("metarecord") && !MetarecordPlugin::build(configuration, verbose)) return 11;
  if (!prebuild_renderers()) return 1;
  if (!generate_database()) return 2;
  if (!generate_assets()) return 3;
  if (configuration.has_plugin("comet") && !(CometPlugin::build(configuration, verbose, clean))) return 10;
  if (configuration.toolchain() == "cmake")
    result = crails_cmake_builder(configuration, mode, cxx_flags, verbose, clean) ? 0 : 4;
  else if (configuration.toolchain() == "build2")
    result = crails_build2_builder(configuration, verbose, clean) ? 0 : 4;
  else
    cerr << "Build command not supported for " << configuration.toolchain() << endl;
  if (result == 0)
  {
    restart_server();
    Crails::cli_notification("Crails Framework", "Build succeeded.", "success");
  }
  else
    Crails::cli_notification("Crails Framework", "Build failed", "failure");
  return result;
}
