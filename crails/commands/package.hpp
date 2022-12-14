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
      ("mode,m",        boost::program_options::value<std::string>(),  "Debug or Release: defaults to the build-type value set in the .crails file")
      ("defines,d",     boost::program_options::value<std::vector<std::string>>()->multitoken(), "custom preproessor defines (such as --defines MY_DEFINE ...)")
      ("output,o",      boost::program_options::value<std::string>(), "output file (defaults to application.tar.gz)")
      ("port,p",        boost::program_options::value<unsigned short>(), "network port the application service will bind to")
      ("name,n",        boost::program_options::value<std::string>(), "alternative application name to use for the deployed application")
      ("install-root",  boost::program_options::value<std::string>(), "expected remote install directory")
      ("skip-tests", "does not condition the building of a package on the success of the test suite");
  }
private:
  bool generate_scripts();
  bool generate_tarball();
  void cleanup_tmp();

  std::string bin_target() const { return install_directory + "/bin/" + configuration.variable("name"); }
  std::string share_target() const { return install_directory + "/share/" + configuration.variable("name"); }
  std::string lib_target() const { return install_directory + "/lib"; }

  std::string install_directory = "/usr/local";
  std::vector<std::filesystem::path> package_files;
};
