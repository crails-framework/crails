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
  if (options.count("dockerfile"))
    shell_command << " --dockerfile \"" << options["dockerfile"].as<string>() << '"';
  shell_command << " -c \"" << command << '"';
  if (options.count("verbose"))
    cout << "+ " << shell_command.str() << endl;
  return Crails::run_command(shell_command.str()) ? 0 : -1;
}

void DockerPlugin::ShellCommand::add_docker_defines(stringstream& crails_command)
{
  crails_command << " --defines CRAILS_DOCKER_BUILD";
  if (options.count("dockerfile"))
  {
    string dockerfile_define = options["dockerfile"].as<string>();
    transform(dockerfile_define.begin(), dockerfile_define.end(), dockerfile_define.begin(), [](char c) { return toupper(c); });
    crails_command << ' ' << dockerfile_define << "_DOCKER_BUILD";
  }
  if (options.count("defines"))
  {
    for (const string& define : options["defines"].as<vector<string>>())
      crails_command << ' ' << define;
  }
}

static void forward_command(stringstream& stream, boost::program_options::variables_map& options, const vector<string>& blacklist)
{
  for (auto it = options.begin() ; it != options.end() ; ++it)
  {
    if (find(blacklist.begin(), blacklist.end(), it->first) == blacklist.end())
    {
      boost::any value = it->second.value();

      stream << " --" << it->first;
      if (value.type() == typeid(std::string))
        stream << ' ' << '"' << it->second.as<std::string>() << '"';
      else if (value.type() == typeid(unsigned short))
        stream << ' ' << it->second.as<unsigned short>();
      else if (value.type() == typeid(std::vector<std::string>))
      {
        for (const std::string& item : it->second.as<std::vector<std::string>>())
          stream << ' ' << '"' << item << '"';
      }
    }
  }
}

int DockerPlugin::DockerBuild::run()
{
  stringstream crails_command;

  crails_command << "crails build";
  if (options.count("verbose"))
    crails_command << " -v";
  if (options.count("mode"))
    crails_command << " -m " << options["mode"].as<string>();
  add_docker_defines(crails_command);
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

  crails_command << "crails package";
  if (options.count("verbose"))
    crails_command << " -v";
  if (options.count("mode"))
    crails_command << " -m " << options["mode"].as<string>();
  else
    crails_command << " -m Release";
  add_docker_defines(crails_command);
  if (options.count("port"))
    crails_command << " --port " << options["port"].as<unsigned short>();
  if (options.count("name"))
    crails_command << " --name " << options["name"].as<string>();
  if (options.count("install-root"))
    crails_command << " --install-root \"" << options["install-root"].as<string>() << '"';
  if (options.count("install-user"))
    crails_command << " --install-user " << options["install-user"].as<string>();
  if (options.count("install-group"))
    crails_command << " --install-group " << options["install-group"].as<string>();
  if (options.count("install-runtime-path"))
    crails_command << " --install-runtime-path \"" << options["install-runtime-path"].as<string>() << '"';
  crails_command << " -o " << temporary_file;
  if (options.count("output"))
    output = options["output"].as<string>();
  result = call_docker_shell_with(crails_command.str());
  if (result == 0)
    filesystem::copy(temporary_file, output, filesystem::copy_options::overwrite_existing);
  return result;
}

int DockerPlugin::DockerDeploy::run()
{
  stringstream crails_command;

  crails_command << "crails deploy";
  forward_command(crails_command, options, {"dockerfile"});
  return call_docker_shell_with(crails_command.str());
}
