#include "build.hpp"
#include <crails/utils/string.hpp>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <crails/utils/string.hpp>

#include "odb.hpp"

using namespace std;

struct WithPath
{
  const boost::filesystem::path old_path;
  WithPath(boost::filesystem::path new_path) : old_path(boost::filesystem::current_path())
  {
    boost::filesystem::create_directories(new_path);
    boost::filesystem::current_path(new_path);
  }
  ~WithPath()
  {
    boost::filesystem::current_path(old_path);
  }
};

bool BuildManager::prebuild_renderers()
{
  for (const auto& renderer : configuration.renderers())
  {
    stringstream command;

    cout << "[renderers] generate " << renderer << "..." << endl;
    command << configuration.crails_bin_path() + "/crails"
      << " templates build -r " << renderer << " -i app/views "
      << " -t Crails::" << Crails::uppercase(renderer) << "Template"
      << " -z crails/" << renderer << "_template.hpp"
      << " -p \\." << renderer << "$";
    boost::process::child process(command.str());
    process.wait();
    if (process.exit_code() != 0)
      return false;
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
    boost::process::child process(command.str());
    process.wait();
    if (process.exit_code() != 0)
      return false;
  }
  return true;
}

bool BuildManager::generate_database()
{
  if (configuration.has_plugin("libcrails-odb"))
  {
    BuildOdb odb_builder;
    std::vector<std::string> argv_array{"--input-dirs","app/models","--output-dir","lib/odb"};
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

bool BuildManager::build_with_cmake()
{
  WithPath path_lock(configuration.application_build_path());
  boost::process::child cmake("cmake " + configuration.project_directory());

  cmake.wait();
  if (cmake.exit_code() == 0)
  {
    boost::process::child make("make");

    make.wait();
    return make.exit_code() == 0;
  }
  return false;
}

int BuildManager::run()
{
  if (!prebuild_renderers()) return false;
  if (!generate_assets()) return false;
  if (!generate_database()) return false;
  if (configuration.toolchain() == "cmake")
    return build_with_cmake() ? 0 : -1;
  else
    cerr << "Build command not supported for " << configuration.toolchain() << endl;
  return -1;
}
