#include <boost/process.hpp>
#include <crails/cli/cmake_builder.hpp>
#include <crails/cli/build2_builder.hpp>
#include <crails/cli/process.hpp>
#include "project_configuration.hpp"
#include <filesystem>
#include <iostream>

using namespace std;

static map<string, string> mode_map{
  {"release", "Release"},
  {"debug", "Debug"}
};

bool crails_cmake_builder(const ProjectConfiguration& configuration, const string& mode, const string& cxx_flags, bool verbose, bool clean)
{
  if (CMakeBuilder::installed())
  {
    int options = 0;

    if (verbose)
      options += BuildVerbose;
    if (clean)
      options += BuildClean;
    CMakeBuilder builder(
      configuration.project_directory(),
      configuration.application_build_path(),
      options
    );

    builder.option("CMAKE_BUILD_TYPE", mode_map.find(mode) != mode_map.end() ? mode_map.at(mode) : mode);
    if (cxx_flags.length() > 0)
      builder.option("CMAKE_CXX_FLAGS:", cxx_flags);
    return builder.build();
  }
  else
    cerr << "cmake does not appear to be installed." << endl;
  return false;
}

bool crails_build2_builder(const ProjectConfiguration& configuration, bool verbose, bool clean)
{
  int options = 0;

  if (verbose)
    options += BuildVerbose;
  if (clean)
    options += BuildClean;
  if (Build2Builder::installed())
  {
    Build2Builder build2(
      configuration.variable_or("name", "application"),
      configuration.project_directory(),
      configuration.application_build_path(),
      options
    );

    if (clean)
      filesystem::remove_all(configuration.application_build_path());
    if (!filesystem::is_directory(configuration.application_build_path()))
      Build2Builder::create(configuration.application_build_path(), {{"config.cxx", "g++"}, {"config.bin.rpath", "/usr/local/lib"}});
    return build2.configure() && build2.build();
  }
  else
    cerr << "bpkg does not appear to be installed." << endl;
  return false;
}
