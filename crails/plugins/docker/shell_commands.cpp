#include "plugin.hpp"
#include <boost/process.hpp>
#include <crails/cli/process.hpp>
#include <filesystem>
#include <iostream>

using namespace std;

static const string temporary_package_file(".docker-package.tar.gz");

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
      if (value.type() == typeid(std::string) && it->second.as<std::string>().length() > 0)
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
  string output = "package.tar.gz";
  int result;

  crails_command << "crails package";
  if (!options.count("mode"))
    crails_command << " -m Release";
  add_docker_defines(crails_command);
  forward_command(crails_command, options, {"dockerfile","defines","output"});
  crails_command << " -o " << temporary_package_file;
  if (options.count("output"))
    output = options["output"].as<string>();
  result = call_docker_shell_with(crails_command.str());
  if (result == 0 && output != temporary_package_file)
    filesystem::copy(temporary_package_file, output, filesystem::copy_options::overwrite_existing);
  return result;
}

int DockerPlugin::DockerDeploy::run()
{
  stringstream crails_command, deploy_command;

  crails_command << configuration.crails_bin_path() << "/crails plugins docker package ";
  deploy_command << configuration.crails_bin_path() << "/crails-deploy ";
  forward_command(crails_command, options, {"sudo","hostname","deploy-user","env","root","user","group","app-port","app-host","runtime-path","pubkey","password","jail-path"});
  forward_command(deploy_command, options, {"dockerfile","mode","skip-tests"});
  crails_command << " --output " << temporary_package_file;
  deploy_command << " --app-name \"" << configuration.variable("name") << '"'
                 << " --start \"/usr/local/bin/" << configuration.variable("name") << "/start.sh\""
                 << " --stop \"/usr/local/bin/" << configuration.variable("name") << "/stop.sh\""
                 << " --package " << temporary_package_file;
  if (options.count("verbose"))
    cout << "+ " << crails_command.str() << endl;
  if (Crails::run_command(crails_command.str()))
  {
    if (options.count("verbose"))
      cout << "+ " << deploy_command.str() << endl;
    if (Crails::run_command(deploy_command.str()))
      return 0;
  }
  return -1;
}
