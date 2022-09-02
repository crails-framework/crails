#include "plugin.hpp"
#include "../../file_renderer.hpp"
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <crails/cli/process.hpp>
#include <crails/cli/with_path.hpp>

using namespace std;

CometPlugin::CometPlugin()
{
  add_command("install", []() { return make_shared<CometInstaller>(); });
}

int CometPlugin::CometInstaller::run()
{
  string comet = find_comet_command(configuration);
  string output_path = "app/client";

  boost::filesystem::create_directories(output_path);
  if (configuration.has_plugin("comet"))
    cerr << "comet plugin already installed" << endl;
  else if (comet.length() == 0)
    cerr << "comet does not seem to be installed on your system" << endl;
  else
  {
    stringstream command;
    string asset_cpp_path = boost::filesystem::relative("lib/assets.cpp", boost::filesystem::canonical(output_path)).string();

    if (options.count("client-path"))
      output_path = options["client-path"].as<string>();
    command << comet << " new "
      << " -n " << configuration.variable("name")
      << " -o " << output_path
      << " -t " << configuration.toolchain()
      << " -b \"" << configuration.application_build_path() + "/client" << '"'
      << " --html-config config/comet.json"
      << " --html-output lib-client"
      << " --include-src \"" << asset_cpp_path << "\"";
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

string CometPlugin::asset_exclusion_pattern(const ProjectConfiguration& configuration)
{
  if (configuration.asset_roots().size() > 0)
  {
    string asset_root = *configuration.asset_roots().begin();
    string define = "__CHEERP_CLIENT__";

    return define
      + ':' + boost::filesystem::path(asset_root + "/application.js").string()
      + ':' + boost::filesystem::path(asset_root + "/application.js.map").string();
  }
  return "err:missing-asset-roots";
}

static bool copy_build_to_assets(const ProjectConfiguration& configuration)
{
  if (configuration.asset_roots().size() > 0)
  {
    string javascript_output = configuration.application_build_path() + "/client/application.js";
    string sourcemaps_output = javascript_output + ".map";
    string asset_root = *configuration.asset_roots().begin();
    boost::system::error_code ec;

    boost::filesystem::create_directories(asset_root, ec);
    if (ec)
    {
      cerr << "cannot create asset directory: " << ec.message() << endl;
      return false;
    }
    if (boost::filesystem::exists(javascript_output)) boost::filesystem::copy(javascript_output, asset_root + "/application.js",     boost::filesystem::copy_options::overwrite_existing);
    if (boost::filesystem::exists(sourcemaps_output)) boost::filesystem::copy(sourcemaps_output, asset_root + "/application.js.map", boost::filesystem::copy_options::overwrite_existing);
    return true;
  }
  else
    cerr << "no asset-roots found in ./.crails" << endl;
  return false;
}

bool CometPlugin::build(const ProjectConfiguration& configuration, bool verbose)
{
  string comet = find_comet_command(configuration);
  string source = configuration.variable("comet-client-path");
  string command = comet + " build";

  if (verbose)
    command += " -v";
  {
    Crails::WithPath with_path(source);

    if (comet.length() < 0)
    {
      cerr << "comet does not seem to be installed on your system" << endl;
      return false;
    }
    if (!Crails::run_command(command))
      return false;
  }
  return copy_build_to_assets(configuration);
}
