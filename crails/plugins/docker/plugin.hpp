#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"

class DockerPlugin : public CommandIndex
{
public:
  static void refresh_environment(const ProjectConfiguration&);

  struct DockerInstaller : public Command
  {
    int run() override;
  };

  struct DockerShell : public Command
  {
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      desc.add_options()
        ("name,n",             boost::program_options::value<std::string>(), "machine name")
        ("dockerfile,i",       boost::program_options::value<std::string>(), "Dockerfile path (defaults to docker/)")
        ("verbose,v",          "verbose docker build (quiet by default)")
        ("noninteractive,a",   "disable interactive mode (must be used when the command is launched from a non-interactive shell)")
        ("application-path,p", boost::program_options::value<std::string>(), "application path (defaults to the current path)")
        ("command,c",          boost::program_options::value<std::string>(), "command to run (defaults to bash)");
    }
  };

  struct DockerPackage : public Command
  {
    int run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {

    }
  };
};
