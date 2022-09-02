#include <boost/process.hpp>
#include <crails/cli/cmake_builder.hpp>
#include "project_configuration.hpp"
#include <filesystem>

using namespace std;

bool crails_cmake_builder(ProjectConfiguration& configuration, bool verbose, bool clean)
{
  int options = 0;

  if (verbose)
    options += CMakeVerbose;
  if (clean)
    options += CMakeClean;
  return CMakeBuilder(
    configuration.project_directory(),
    configuration.application_build_path(),
    options
  ).option("CMAKE_BUILD_TYPE", configuration.build_type())
   .build();
}
