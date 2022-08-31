#include "plugin.hpp"
#include "../../file_renderer.hpp"
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <crails/cli/process.hpp>

using namespace std;

CometPlugin::CometPlugin()
{
  add_command("install", []() { return make_shared<CometInstaller>(); });
}

int CometPlugin::CometInstaller::run()
{
  string comet = find_comet_command(configuration);

  if (configuration.has_plugin("comet"))
    cerr << "comet plugin already installed" << endl;
  else if (comet.length() == 0)
    cerr << "comet does not seem to be installed on your system" << endl;
  else
  {
    stringstream command;
    string output_path = "app/client";

    if (options.count("client-path"))
      output_path = options["client-path"].as<string>();
    command << comet << " new "
      << " -n " << configuration.variable("name")
      << " -o " << output_path
      << " -t " << configuration.toolchain()
      << " --html-config config/comet.json"
      << " --html-output lib-client";
    if (options.count("cheerp-path"))
      command << " --cheerp-path " << options["cheerp-path"].as<string>();
    if (Crails::run_command(command.str()))
    {
      configuration.add_plugin("comet");
      configuration.variable("comet-client-path", output_path);
      configuration.save();
      return 0;
    }
  }
  return -1;
}

string CometPlugin::find_comet_command(const ProjectConfiguration& configuration)
{
  string default_path = configuration.crails_bin_path() + "/comet";

  if (!boost::filesystem::exists(default_path))
    return Crails::which("comet");
  return default_path;
}

bool CometPlugin::build(const ProjectConfiguration& configuration, bool verbose)
{
  string comet = find_comet_command(configuration);
  string source = configuration.variable("comet-client-path");
  string command = comet + " build";

  if (verbose)
    command += " -v";
  boost::filesystem::current_path(source);
  if (comet.length() > 0)
    return Crails::run_command(command);
  else
    cerr << "comet does not seem to be installed on your system" << endl;
  return false;
}
