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

void build2_configure_libcrails_odb(Build2Builder&, const ProjectConfiguration& configuration);

bool crails_build2_builder(const ProjectConfiguration& configuration, bool verbose, bool clean)
{
  bool configured = false;
  int options = 0;

  if (verbose)
    options += BuildVerbose;
  if (clean)
    options += BuildClean;
  if (Build2Builder::installed())
  {
    if (clean)
      filesystem::remove_all(configuration.application_build_path());
    if (!filesystem::is_directory(configuration.application_build_path()))
    {
      map<string,string> options{
        {"config.cxx", configuration.variable_or("compiler", "g++")},
        {"config.cxx.std", configuration.variable_or("std", "latest")}
      };

#ifndef _WIN32
      options.emplace("config.bin.rpath", "/usr/local/lib");
#endif
      Build2Builder::create(configuration.application_build_path(), options, verbose);
    }
    else
      configured = true;

    Build2Builder build2(
      "lib" + configuration.variable_or("name", "application"),
      configuration.project_directory(),
      configuration.application_build_path(),
      options
    );

    if (configuration.variable("build-with-system-packages") == "1")
    {
      build2.use_system_packages(configuration.plugins());
      build2.use_system_package("libcrails-tests");
    }
    if (clean || !configured)
    {
      if (build2.configure())
      {
        build2_configure_libcrails_odb(build2, configuration);
      }
      else
      {
        filesystem::remove_all(configuration.application_build_path());
        return false;
      }
    }
    return build2.build();
  }
  else
    cerr << "bpkg does not appear to be installed." << endl;
  return false;
}
