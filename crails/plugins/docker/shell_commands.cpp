#include "plugin.hpp"
#include <boost/process.hpp>
#include <crails/cli/process.hpp>
#include <filesystem>
#include <iostream>

using namespace std;

int DockerPlugin::ShellCommand::call_docker_shell_with(const string& command)
{
  stringstream shell_command;

  shell_command << configuration.crails_bin_path() << "/crails p docker shell";
  if (options.count("verbose"))
    shell_command << " -v";
  if (options.count("name"))
    shell_command << " -n " << options["name"].as<string>();
  if (options.count("dockerfile"))
    shell_command << " --dockerfile \"" << options["dockerfile"].as<string>() << '"';
  shell_command << " -c \"" << command << '"';
  if (options.count("verbose"))
    cout << "+ " << shell_command.str() << endl;
  return Crails::run_command(shell_command.str()) ? 0 : -1;
}

int DockerPlugin::DockerBuild::run()
{
  stringstream crails_command;

  crails_command << "crails build";
  if (options.count("verbose"))
    crails_command << " -v";
  if (options.count("mode"))
    crails_command << " -m " << options["mode"].as<string>();
  return call_docker_shell_with(crails_command.str());
}

int DockerPlugin::DockerRun::run()
{
  stringstream crails_command;

  crails_command << "build/server";
  if (options.count("port"))
    crails_command << " -p " << options["port"].as<unsigned short>();
  return call_docker_shell_with(crails_command.str());
}

int DockerPlugin::DockerPackage::run()
{
  stringstream crails_command;
  string temporary_file = ".docker-package.tar.gz";
  string output = "package.tar.gz";
  int result;

  crails_command << configuration.crails_bin_path() << "/crails package";
  if (options.count("verbose"))
    crails_command << " -v";
  if (options.count("mode"))
    crails_command << " -m " << options["mode"].as<string>();
  else
    crails_command << " -m Release";
  if (options.count("port"))
    crails_command << " --port " << options["port"].as<unsigned short>();
  if (options.count("name"))
    crails_command << " --name " << options["name"].as<string>();
  if (options.count("install-root"))
    crails_command << " --install-root \"" << options["install-root"].as<string>() << '"';
  if (options.count("install-user"))
    crails_command << " --install-user \"" << options["install-user"].as<string>() << '"';
  if (options.count("install-group"))
    crails_command << " --install-group \"" << options["install-group"].as<string>() << '"';
  if (options.count("install-runtime-path"))
    crails_command << " --install-runtime-path \"" << options["install-runtime-path"].as<string>() << '"';
  crails_command << " -o " << temporary_file;
  if (options.count("output"))
    output = options["output"].as<string>();
  result = call_docker_shell_with(crails_command.str());
  if (result == 0)
    filesystem::copy(temporary_file, output);
  return result;
}
