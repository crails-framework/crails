#include "build.hpp"
#include <crails/utils/string.hpp>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <crails/utils/string.hpp>

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
  std::cout << "prebuild renderers " << configuration.renderers().size() << std::endl;
  for (const auto& renderer : configuration.renderers())
  {
    stringstream command;

    command << configuration.crails_bin_path() + "/crails"
      << " templates build -r " << renderer << " -i app/views "
      << " -t Crails::" << Crails::uppercase(renderer) << "Template"
      << " -z crails/" << renderer << "_template.hpp";
    boost::process::child process(command.str());
    process.wait();
    if (process.exit_code() != 0)
      return false;
  }
  return true;
}

bool BuildManager::generate_assets()
{
  if (boost::filesystem::exists(configuration.crails_bin_path() + "crails-assets"))
  {
    std::stringstream command;

    command << configuration.crails_bin_path() + "/crails-assets"
      << "-i " << Crails::join(configuration.asset_roots(), ',')
      << "-o public";
    boost::process::child process(command.str());
    process.wait();
    if (process.exit_code() != 0)
      return false;
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
  configuration.initialize();
  if (!prebuild_renderers()) return false;
  if (!generate_assets()) return false;
  if (configuration.toolchain() == "cmake")
    return build_with_cmake() ? 0 : -1;
  else
    cerr << "Build command not supported for " << configuration.toolchain() << endl;
  return -1;
}
