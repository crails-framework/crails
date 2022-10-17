#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"
#include <sstream>

class DockerPlugin : public CommandIndex
{
public:
  DockerPlugin();

  std::string_view description() const override { return "generates and interact with docker machines to build, test and package your application for a given target"; }

  static void refresh_environment(const ProjectConfiguration&);

  struct DockerInstaller : public ::Command
  {
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("image", boost::program_options::value<std::string>(), "docker image to use (defaults to debian:sid)");
    }
  };

  struct DockerNew : public ::Command
  {
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("image", boost::program_options::value<std::string>(), "docker image to use (defaults to debian:sid)")
        ("name",  boost::program_options::value<std::string>(), "name for the new docker image");
    }
    virtual std::string_view description() const override { return "creates a new docker machine"; }
  };

  struct DockerRefreshEnvironment : public ::Command
  {
    int run() override { DockerPlugin::refresh_environment(configuration); return 0; }
    virtual std::string_view description() const override { return "update crails dependencies in the crails machines"; }
  };

  struct DockerShell : public ::Command
  {
    std::string_view description() const override { return "opens an interactive shell into a docker machine"; }
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("dockerfile,i",       boost::program_options::value<std::string>(), "Dockerfile path (defaults to base)")
        ("verbose,v",          "verbose docker build (quiet by default)")
        ("noninteractive,a",   "disable interactive mode (must be used when the command is launched from a non-interactive shell)")
        ("application-path,p", boost::program_options::value<std::string>(), "application path (defaults to the current path)")
        ("command,c",          boost::program_options::value<std::string>(), "command to run (defaults to bash)");
    }
  };

  struct ShellCommand : public ::Command
  {
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("dockerfile,i", boost::program_options::value<std::string>(), "Dockerfile path (defaults to docker/base)")
        ("verbose,v", "verbose mode (quiet by default)");
    }
  protected:
    int call_docker_shell_with(const std::string& command);
    void add_docker_defines(std::stringstream&);
  };

  struct DockerBuild : public ShellCommand
  {
    std::string_view description() const override { return "builds the application from a docker machine"; }
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      ShellCommand::options_description(desc);
      desc.add_options()
        ("mode,m", boost::program_options::value<std::string>(), "Release or Debug, defaults to build-type value in .crails file")
        ("defines", boost::program_options::value<std::vector<std::string>>()->multitoken(), "custom preprocessor defines (such as --defines MY_DEFINE ...)");
    }
  };

  struct DockerRun : public ShellCommand
  {
    std::string_view description() const override { return "runs the application from a docker machine"; }
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      ShellCommand::options_description(desc);
      desc.add_options()
        ("port,p", boost::program_options::value<unsigned short>(), "port to bind");
    }
  };

  struct DockerPackage : public ShellCommand
  {
    std::string_view description() const override { return "generates an exportable package of your application"; }
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      ShellCommand::options_description(desc);
      desc.add_options()
        ("mode,m",        boost::program_options::value<std::string>(), "build mode (Release or Debug)")
        ("defines", boost::program_options::value<std::vector<std::string>>()->multitoken(), "custom preprocessor defines (such as --defines MY_DEFINE ...)")
        ("output,o",      boost::program_options::value<std::string>(), "output path for the package")
        ("port,p",        boost::program_options::value<unsigned short>(), "network port the application service will bind to")
        ("name,n",        boost::program_options::value<std::string>(), "alternative application name to use for the deployed application")
        ("install-root",  boost::program_options::value<std::string>(), "expected remote install directory")
        ("skip-tests", "does not condition the building of a package on the success of the test suite");
    }
  };

  struct DockerDeploy : public ShellCommand
  {
    std::string_view description() const override { return "deploys your application to a remote unix-like server"; }
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      using namespace std;
      ShellCommand::options_description(desc);
      desc.add_options()
        ("sudo",          "administrative task will require root permissions")
        ("mode,m",        boost::program_options::value<std::string>(), "build mode (Release or Debug)")
        ("hostname,o",    boost::program_options::value<string>(), "deployment target")
        ("deploy-user,d", boost::program_options::value<string>(), "user performing the deployment")
        ("root,r",        boost::program_options::value<string>(), "remote install directory")
        ("user,u",        boost::program_options::value<string>(), "user which will run the application")
        ("group,g",       boost::program_options::value<string>(), "user group which will run the application")
        ("app-port,p",    boost::program_options::value<unsigned short>(), "network port the application service will bind to")
        ("app-host",      boost::program_options::value<std::string>(), "network host the application service will bind to")
        ("runtime-path",  boost::program_options::value<string>(), "runtime path (defaults to /var/application-name)")
        ("env,e",         boost::program_options::value<string>(), "environment file to be used by the deployed application")
        ("pubkey", "ssh authentication using rsa public key")
        ("password",      boost::program_options::value<string>(), "password used for ssh authentication (using the SSH_PASSWORD environment variable will be more secure than this CLI option)")
        ("jail-path",     boost::program_options::value<string>(), "freebsd jail path")
        ("skip-tests", "does not condition the building of a package on the success of the test suite");
    }
  };
};
