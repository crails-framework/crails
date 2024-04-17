#include "plugin.hpp"
#include <boost/process.hpp>
#include <crails/cli/process.hpp>
#include <crails/utils/join.hpp>
#include <crails/utils/split.hpp>
#include <filesystem>
#include <iostream>

using namespace std;

static const string workdir = "docker";

static filesystem::path get_application_path(const boost::program_options::variables_map& options)
{
  return options.count("application-path")
    ? options["application-path"].as<string>()
    : filesystem::current_path().string();
}

static filesystem::path get_build_path(const string& machine_name, const boost::program_options::variables_map& options)
{
  return get_application_path(options) / "docker" / "build" / machine_name;
}

static map<string,string> get_docker_volumes(const string& machine_name, const boost::program_options::variables_map& options, const ProjectConfiguration& configuration)
{
  map<string,string> paths{
    {get_application_path(options).string(),         "/opt/application"},
    {get_build_path(machine_name, options).string(), "/opt/application/build"}
  };
  string configured_paths = configuration.variable("docker-paths");

  for (const string& token : Crails::split(configured_paths, ';'))
  {
    const auto parts = Crails::split(token, ':');

    if (parts.size() == 2)
    {
      error_code err;
      filesystem::path local_path = filesystem::canonical(*parts.begin(), err);

      if (!err)
        paths.emplace(local_path.string(), *parts.rbegin());
      else
        cerr << "invalid docker-path: " << token << endl;
    }
    else
      cerr << "malformed docker-path: " << token << endl;
  }
  return paths;
}

static vector<string> make_shell_command(const string& machine_name, const boost::program_options::variables_map& options, const ProjectConfiguration& configuration)
{
  vector<string> shell_command;
  string application_path = get_application_path(options).string();
  string build_path = get_build_path(machine_name, options).string();
  string command = options.count("command")
    ? options["command"].as<string>()
    : "bash";
  map<string,string> docker_volumes = get_docker_volumes(machine_name, options, configuration);

  filesystem::create_directories(build_path);
  shell_command.push_back("run");
  shell_command.push_back("--net=host");
  shell_command.push_back(!options.count("noninteractive") ? "-it" : "-t");
  shell_command.push_back("--rm");
  for (auto it = docker_volumes.begin() ; it != docker_volumes.end() ; ++it)
  {
    shell_command.push_back("-v");
    shell_command.push_back(it->first + ':' + it->second);
  }
  shell_command.push_back(machine_name);
  for (const string& part : Crails::split(command, ' '))
    shell_command.push_back(part);
  if (options.count("verbose"))
    cout << "+ docker " << Crails::join(shell_command, ' ') << endl;
  return shell_command;
}

static string make_dockerfile_path(const boost::program_options::variables_map& options)
{
  string dockerfile_path = workdir + "/base";

  if (options.count("dockerfile"))
    dockerfile_path  = workdir + '/' + options["dockerfile"].as<string>();
  return dockerfile_path;
}

static string make_build_command(const string& machine_name, const boost::program_options::variables_map& options)
{
  stringstream command;
  string dockerfile_path = make_dockerfile_path(options);

  command << Crails::which("docker") << " build";
  if (!options.count("verbose"))
    command << " -q";
  command << " -t \"" << machine_name << "\" -f \"" << dockerfile_path << "/Dockerfile\"" << " \"" << workdir << '"';
  if (options.count("verbose"))
    cout << "+ " << command.str() << endl;
  return command.str();
}

static string make_machine_name(const boost::program_options::variables_map& options)
{
  string machine_name;

  return filesystem::path(make_dockerfile_path(options)).filename().string();
}

int DockerPlugin::DockerShell::run()
{
  string machine_name = make_machine_name(options);
  string docker_path  = Crails::which("docker");

  DockerPlugin::refresh_environment(configuration);
  if (docker_path.length() == 0)
    cerr << "cannot find docker. Maybe it's not installed ?" << endl;
  else
  {
    boost::process::child build_process(make_build_command(machine_name, options));

    build_process.wait();
    if (build_process.exit_code() == 0)
      return Crails::execve(docker_path, make_shell_command(machine_name, options, configuration));
  }
  return -1;
}
