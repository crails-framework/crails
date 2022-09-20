#pragma once
#include "../command.hpp"
#include <filesystem>

class Package : public ::Command
{
public:
  std::string_view description() const override { return "Packages your crails application for deployment."; }
  int              run() override;
  void             options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("verbose,v", "enable verbose mode")
      ("mode,m",    "Debug or Release: defaults to the build-type value set in the .crails file")
      ("output,o",  "output file (defaults to application.tar.gz)")
      ("install-root",  "expected remote install directory")
      ("install-user",  "user which should run the application in the runtime environment")
      ("install-group", "user group which should run the application in the runtime environment");
  }
private:
  bool generate_scripts();
  bool generate_tarball();

  std::string bin_target() const { return install_directory + "/bin/" + configuration.variable("name"); }
  std::string share_target() const { return install_directory + "/share/" + configuration.variable("name"); }
  std::string lib_target() const { return install_directory + "/lib"; }
  std::string pidfile_target() const { return "/tmp/" + configuration.variable("name") + ".pid"; }

  std::string install_directory = "/usr/local";
  std::vector<std::filesystem::path> package_files;
};
