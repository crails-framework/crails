#include "plugin.hpp"
#include <boost/process.hpp>
#include <crails/cli/process.hpp>
#include <filesystem>
#include <iostream>

using namespace std;

static vector<string> make_shell_command(const string& machine_name, const boost::program_options::variables_map& options)
{
  vector<string> shell_command;
  string application_path = options.count("application-path")
    ? options["application-path"].as<string>()
    : filesystem::current_path().string();
  string build_path = application_path + "/docker/build-" + machine_name;
  string command = options.count("command")
    ? options["command"].as<string>()
    : "bash";

  filesystem::create_directories(build_path);
  shell_command.push_back("run");
  shell_command.push_back("--net-host");
  shell_command.push_back(!options.count("noninteractive") ? "-it" : "-t");
  shell_command.push_back("--rm");
  shell_command.push_back("-v");
  shell_command.push_back(application_path + ':' + "/opt/application");
  shell_command.push_back("-v");
  shell_command.push_back(build_path + ':' + "/opt/application/build");
  shell_command.push_back(machine_name);
  shell_command.push_back(command);
  return shell_command;
}

static string make_build_command(const string& machine_name, const boost::program_options::variables_map& options)
{
  stringstream command;
  string dockerfile_path = "docker/base";

  if (options.count("dockerfile"))
    dockerfile_path  = options["dockerfile"].as<string>();
  command << Crails::which("docker") << " build";
  if (!options.count("verbose"))
    command << " -q";
  command << " -t \"" << machine_name << "\" \"" << dockerfile_path << '\"';
  return command.str();
}

int DockerPlugin::DockerShell::run()
{
  string machine_name = options.count("name") ? options["name"].as<string>() : string("crails_docker");
  string docker_path  = Crails::which("docker");

  DockerPlugin::refresh_environment(configuration);
  if (docker_path.length() == 0)
    cerr << "cannot find docker. Maybe it's not installed ?" << endl;
  else
  {
    boost::process::child build_process(make_build_command(machine_name, options));

    build_process.wait();
    if (build_process.exit_code())
      return Crails::execve(docker_path, make_shell_command(machine_name, options));
  }
  return -1;
}

static int call_docker_shell_with(const ProjectConfiguration& configuration, const string& command, const boost::program_options::variables_map& options)
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
  return call_docker_shell_with(configuration, crails_command.str(), options);
}

int DockerPlugin::DockerRun::run()
{
  stringstream crails_command;

  crails_command << "build/server";
  if (options.count("port"))
    crails_command << " -p " << options["port"].as<unsigned short>();
  return call_docker_shell_with(configuration, crails_command.str(), options);
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
  crails_command << " -o " << temporary_file;
  if (options.count("output"))
    output = options["output"].as<string>();
  result = call_docker_shell_with(configuration, crails_command.str(), options);
  if (result == 0)
    filesystem::copy(temporary_file, output);
  return result;
}
