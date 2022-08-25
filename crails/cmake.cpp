#include <boost/process.hpp>
#include "project_configuration.hpp"
#include "with_path.hpp"

using namespace std;

class CMakeBuilder : private WithPath
{
  boost::filesystem::path project_directory;
public:
  CMakeBuilder(const boost::filesystem::path& project_directory, const boost::filesystem::path& build_directory) :
    WithPath(build_directory),
    project_directory(project_directory)
  {
  }

  bool configure()
  {
    boost::process::child cmake("cmake " + project_directory.string());

    cmake.wait();
    return cmake.exit_code() == 0;
  }

  bool make()
  {
    boost::process::child make("make");

    make.wait();
    return make.exit_code() == 0;
  }

  bool build()
  {
    return configure() && make();
  }
};

static bool build_server(ProjectConfiguration& configuration)
{
  return CMakeBuilder(
    configuration.project_directory(),
    configuration.application_build_path()
  ).build();
}

static bool build_client(ProjectConfiguration& configuration)
{
  if (configuration.has_plugin("comet"))
  {
    return CMakeBuilder(
      configuration.variable("comet-path"),
      *configuration.asset_roots().begin()
    ).build();
  }
  return true;
}

bool crails_cmake_builder(ProjectConfiguration& configuration)
{
  return build_server(configuration) && build_client(configuration);
}
