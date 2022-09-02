#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"

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

  struct DockerShell : public ::Command
  {
    std::string_view description() const override { return "opens an interactive shell into a docker machine"; }
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("name,n",             boost::program_options::value<std::string>(), "machine name")
        ("dockerfile,i",       boost::program_options::value<std::string>(), "Dockerfile path (defaults to docker/base)")
        ("verbose,v",          "verbose docker build (quiet by default)")
        ("noninteractive,a",   "disable interactive mode (must be used when the command is launched from a non-interactive shell)")
        ("application-path,p", boost::program_options::value<std::string>(), "application path (defaults to the current path)")
        ("command,c",          boost::program_options::value<std::string>(), "command to run (defaults to bash)");
    }
  };

  struct DockerBuild : public ::Command
  {
    std::string_view description() const override { return "builds the application from a docker machine"; }
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("name,n", boost::program_options::value<std::string>(), "machine name")
        ("dockerfile,i", boost::program_options::value<std::string>(), "Dockerfile path (defaults to docker/base)")
        ("verbose,v", "verbose mode (quiet by default)");
    }
  };

  struct DockerRun : public ::Command
  {
    std::string_view description() const override { return "runs the application from a docker machine"; }
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("name,n",       boost::program_options::value<std::string>(),    "machine name")
        ("dockerfile,i", boost::program_options::value<std::string>(),    "Dockerfile path (defaults to docker/base)")
        ("port,p",       boost::program_options::value<unsigned short>(), "port to bind");
    }
  };

  struct DockerPackage : public ::Command
  {
    std::string_view description() const override { return "generates an exportable package of your application"; }
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("name,n",       boost::program_options::value<std::string>(), "machine name")
        ("dockerfile,i", boost::program_options::value<std::string>(), "Dockerfile path (defaults to docker/base)")
        ("output,o",     boost::program_options::value<std::string>(), "output path for the package")
        ("verbose,v", "verbose mode (quiet by default)");
    }
  };
};
