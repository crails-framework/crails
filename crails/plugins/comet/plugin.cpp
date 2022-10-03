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
      << " --html-config ../config/comet.json"
      << " --html-output ../lib-client"
      << " --include-src \"" << asset_cpp_path << "\"";
    if (options.count("cheerp-path"))
      command << " --cheerp-path " << options["cheerp-path"].as<string>();
    cout << "+ " << command.str() << endl;
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
      + ':' + boost::filesystem::path(configuration.application_build_path() + "/client/application.js").string()
      + ':' + boost::filesystem::path(configuration.application_build_path() + "/client/application.js.map").string();
  }
  return "err:missing-asset-roots";
}

string CometPlugin::assets_command_options(const ProjectConfiguration& configuration)
{
  stringstream stream;
  string application_js     = configuration.application_build_path() + "/client/application.js";
  string application_wasm   = configuration.application_build_path() + "/client/application.wasm";
  string application_js_map = configuration.application_build_path() + "/client/application.js.map";

  if (boost::filesystem::exists(application_js))
    stream << " -i \"" << application_js << '"';
  if (boost::filesystem::exists(application_wasm))
    stream << " -i \"" << application_wasm << '"';
  if (configuration.variable_or("build-type", "Release") == "Debug"
   && boost::filesystem::exists(application_js_map))
    stream << " -i \"" << application_js_map << '"';
  stream << " --ifndef " << asset_exclusion_pattern(configuration);
  return stream.str();
}

bool CometPlugin::update_assets(const ProjectConfiguration& configuration, bool verbose)
{
  if (boost::filesystem::exists(configuration.crails_bin_path() + "/crails-assets"))
  {
    std::stringstream command;

    cout << "[comet] update assets..." << endl;
    command << configuration.crails_bin_path() + "/crails-assets"
      << " -o public"
      << " -u"
      << CometPlugin::assets_command_options(configuration);
    if (verbose)
      cout << "+ " << command.str() << endl;
    return Crails::run_command(command.str());
  }
  return true;
}

bool CometPlugin::build(const ProjectConfiguration& configuration, bool verbose, bool clean)
{
  string comet = find_comet_command(configuration);
  string source = configuration.variable("comet-client-path");
  string command = comet + " build";

  if (verbose)
    command += " -v";
  if (clean)
    command += " --clean";
  command += " -m " + configuration.variable_or("build-type", "Release");
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
  return update_assets(configuration, verbose);
}
