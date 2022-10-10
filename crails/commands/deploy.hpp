#pragma once
#include "../command.hpp"
#include <filesystem>
#include <crails/cli/command.hpp>
#include <crails/cli/process.hpp>
#include <iostream>

class Deploy : public ::Command
{
  std::string tarball;
  std::string hostname;
  std::string install_root = "/usr/local";
  std::string runtime_path;
  std::string app_user, app_group;
public:
  static std::string deploy_bin_path()
  {
    return ProjectConfiguration::crails_bin_path() + "/crails-deploy";
  }

  static bool is_available()
  {
    return std::filesystem::is_regular_file(deploy_bin_path());
  }

  std::string_view description() const override { return "Deploys your application to a remote unix-like server"; }

  void options_description(boost::program_options::options_description& desc) const override
  {
    using namespace std;
    desc.add_options()
      ("verbose,v",     "enable verbose mode")
      ("sudo",          "administrative task will require root permissions")
      ("hostname,o",    boost::program_options::value<string>(), "deployment target")
      ("deploy-user,d", boost::program_options::value<string>(), "user performing the deployment")
      ("root,r",        boost::program_options::value<string>(), "remote install directory")
      ("user,u",        boost::program_options::value<string>(), "user which will run the application")
      ("group,g",       boost::program_options::value<string>(), "user group which will run the application")
      ("runtime-path",  boost::program_options::value<string>(), "runtime path (defaults to /var/application-name)")
      ("env,e",         boost::program_options::value<string>(), "environment file to be used by the deployed application")
      ("pubkey", "ssh authentication using rsa public key")
      ("password",      boost::program_options::value<string>(), "password used for ssh authentication (using the SSH_PASSWORD environment variable will be more secure than this CLI option)")
      ("jail-path",     boost::program_options::value<string>(), "freebsd jail path")
      ("mode",          boost::program_options::value<string>(), "build mode (defaults to Release)")
      ("defines",       boost::program_options::value<string>(), "custom preproessor defines (such as --defines MY_DEFINE ...)")
      ("port",          boost::program_options::value<unsigned short>(), "network port the application service will bind to")
      ("skip-tests", "does not condition the building of a package on the success of the test suite");
  }

  int run() override
  {
    int result;

    if (!options.count("hostname"))
    {
      std::cerr << "missing required argument --hostname" << std::endl;
      return -2;
    }
    hostname = options["hostname"].as<std::string>();
    if (options.count("root"))
      install_root = options["root"].as<std::string>();
    tarball = "package." + hostname + ".tar.gz";
    runtime_path = "/var/" + configuration.variable("name");
    if (options.count("runtime-path"))
      runtime_path = options["runtime-path"].as<std::string>();
    app_user  = options.count("user") ? options["user"].as<std::string>() : configuration.variable("name");
    app_group = options.count("group") ? options["group"].as<std::string>() : app_user;
    if (run_package_command())
      return run_deploy_command() ? 0 : -1;
    return -2;
  }

private:
  bool run_package_command()
  {
    std::stringstream command;

    command << ProjectConfiguration::crails_bin_path() << "/crails";
    command << " package";
    if (options.count("verbose"))
      command << " -v";
    command << " -m Release";
    command << " -o \"" + tarball + '"';
    command << " --install-root \"" << install_root << '"';
    command << " --install-user " << app_user;
    command << " --install-group " << app_group;
    command << " --install-runtime-path \"" << runtime_path << '"';
    if (options.count("port"))
      command << " --port " << options["port"].as<unsigned short>();
    if (options.count("mode"))
      command << " --mode " << options["mode"].as<std::string>();
    if (options.count("skip-tests"))
      command << " --skip-tests";
    if (options.count("defines"))
    {
      command << " --defines";
      for (const std::string& def : options["defines"].as<std::vector<std::string>>())
        command << ' ' << def;
    }
    if (options.count("env"))
      command << " --env \"" << options["env"].as<std::string>() << '"';
    if (options.count("verbose"))
      std::cout << "+ " << command.str() << std::endl;
    return Crails::run_command(command.str());
  }

  bool run_deploy_command()
  {
    std::stringstream command;

    command << deploy_bin_path();
    if (options.count("verbose"))
      command << " -v";
    if (options.count("sudo"))
      command << " --sudo";
    command << " -p \"" << tarball << '"'
      << " --hostname \"" << hostname << '"'
      << " --root \"" << install_root << '"'
      << " --user \"" << app_user << '"'
      << " --group \"" << app_group << '"'
      << " --runtime-path \"" << runtime_path << '"';
    if (options.count("deploy-user"))
      command << " --deploy-user " << options["deploy-user"].as<std::string>();
    if (options.count("pubkey"))
      command << " --pubkey";
    if (options.count("password"))
      command << " --password \"" << options["password"].as<std::string>() << '"';
    if (options.count("jail-path"))
      command << " --jail-path \"" << options["jail-path"].as<std::string>() << '"';
    if (options.count("verbose"))
      std::cout << "+ " << command.str() << std::endl;
    return Crails::run_command(command.str());
  }
};
