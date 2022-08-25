#include "plugin.hpp"
#include "../../file_renderer.hpp"
#include <boost/process.hpp>
#include <boost/filesystem.hpp>

using namespace std;

CometPlugin::CometPlugin()
{
  add_command("install", []() { return make_shared<CometInstaller>(); });
}

int CometPlugin::CometInstaller::run()
{
  FileRenderer renderer;
  boost::filesystem::path path("app/comet");
  boost::filesystem::path vendor_path("vendor/comet");
  std::string relative_vendor_path;

  if (!options.count("cheerp-path"))
  {
    std::cerr << "Option --cheerp-path required." << std::endl;
    return -1;
  }
  if (find_comet_html_generator().length() == 0)
  {
    std::cerr << "comet-html not found. Please install comet using `gem install comet-cpp`" << std::endl;
    return -1;
  }
  boost::filesystem::create_directories(path);
  boost::filesystem::create_directories(vendor_path);
  if (options.count("comet-path"))
    path = options["comet-path"].as<string>();
  relative_vendor_path = boost::filesystem::relative(vendor_path, path).string();
  renderer.vars["project_name"] = configuration.variable("name");
  renderer.vars["relative_vendor_path"] = relative_vendor_path;
  renderer.generate_file("app/comet/CMakeLists.txt");
  configuration.variable("cheerp-path", options["cheerp-path"].as<string>());
  configuration.variable("comet-path", path.string());
  configuration.save();
  return 0;
}

std::string render_comet_cmakelists_txt(const Crails::Renderer*, Crails::SharedVars& vars)
{
  std::string relative_vendor_path = Crails::cast<std::string>(vars, "relative_vendor_path");
  return
    "cmake_minimum_required(VERSION 3,0)\n"
    "\n"
    "project(" + Crails::cast<std::string>(vars, "project_name") + ")\n"
    "include_directories(" + relative_vendor_path + ")\n"
    "\n"
    "if (CMAKE_BUILD_TYPE EQUAL \"DEBUG\")\n"
    "  set(CMAKE_EXE_LINKER_FLAGS \"${CMAKE_EXE_LINKER_FLAGS~} -cheerp-sourcemap=application.js.map -cheerp-sourcemap-standalone\")\n"
    "endif()\n"
    "\n"
    "file(GLOB_RECURSE vendor_src " + relative_vendor_path + "/*.cpp " + relative_vendor_path + "/*.cxx)\n"
    "file(GLOB_RECURSE application_src *.cpp *.cxx)\n"
    "\n"
    "add_executable(application ${vendor_src} ${application_src})";
}

string CometPlugin::find_comet_html_generator()
{
  boost::process::ipstream pipe_stream;
  boost::process::child process("which comet-html", boost::process::std_out > pipe_stream);
  string path;

  process.wait();
  if (process.exit_code() == 0)
    getline(pipe_stream, path);
  return path;
}

bool CometPlugin::generate_comet_views(const ProjectConfiguration& configuration)
{
  string script = find_comet_html_generator();

  if (script.length() > 0)
  {
    string source = configuration.variable("comet-path");
    string output = "app/assets";
    string config = "config/comet.json";
    boost::process::child process(script + " -i " + source + " -o " + output + " -c " + config);

    process.wait();
    return process.exit_code() == 0;
  }
  else
    cerr << "comet-html not found. Perhaps it is not installed, or not reachable via the PATH environment variable ?" << endl;
  return false;
}
