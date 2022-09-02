#include "plugin.hpp"
#include <boost/process.hpp>
#include <crails/cli/process.hpp>
#include <crails/utils/join.hpp>
#include <crails/utils/split.hpp>
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
  shell_command.push_back("--net=host");
  shell_command.push_back(!options.count("noninteractive") ? "-it" : "-t");
  shell_command.push_back("--rm");
  shell_command.push_back("-v");
  shell_command.push_back(application_path + ':' + "/opt/application");
  shell_command.push_back("-v");
  shell_command.push_back(build_path + ':' + "/opt/application/build");
  shell_command.push_back(machine_name);
  for (const string& part : Crails::split(command, ' '))
    shell_command.push_back(part);
  if (options.count("verbose"))
    cout << "+ docker " << Crails::join(shell_command, ' ') << endl;
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
  if (options.count("verbose"))
    cout << "+ " << command.str() << endl;
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
    if (build_process.exit_code() == 0)
      return Crails::execve(docker_path, make_shell_command(machine_name, options));
  }
  return -1;
}
