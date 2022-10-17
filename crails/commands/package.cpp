#include "package.hpp"
#include <crails/cli/process.hpp>
#include <crails/utils/split.hpp>
#include <boost/process.hpp>
#include <algorithm>
#include <iostream>
#include "../file_renderer.hpp"

using namespace std;

struct CleanupFunctor
{
  CleanupFunctor(std::function<void()> callback) : callback(callback) {}
  ~CleanupFunctor() { callback(); }
private:
  std::function<void()> callback;
};

static bool build_command(const ProjectConfiguration& configuration, boost::program_options::variables_map& options)
{
  stringstream command;

  command << configuration.crails_bin_path() << "/crails build";
  if (options.count("verbose"))
    command << " -v";
  if (options.count("mode"))
    command << " -m " << options["mode"].as<string>();
  if (options.count("defines"))
  {
    command << " --defines";
    for (const string& define : options["defines"].as<std::vector<std::string>>())
      command << ' ' << define;
  }
  if (options.count("verbose"))
    cout << "+ " << command.str() << endl;
  return Crails::run_command(command.str());
}

static bool run_tests(const ProjectConfiguration& configuration, boost::program_options::variables_map& options)
{
  if (!options.count("skip-tests"))
  {
    stringstream command;

    command << configuration.application_build_path() << "/tests";
    cout << "+ " << command.str() << endl;
    return Crails::run_command(command.str());
  }
  return true;
}

static vector<filesystem::path> find_imported_libraries(const filesystem::path& binary)
{
  boost::process::ipstream stream;
  boost::process::child process("ldd \"" + binary.string() + '"', boost::process::std_out > stream);
  vector<filesystem::path> results;

  process.wait();
  if (process.exit_code() == 0)
  {
    string line;

    while (getline(stream, line))
    {
      auto list = Crails::split<string, vector<string>>(line, ' ');

      if (list.size() > 2)
        results.push_back(list.at(2));
    }
  }
  return results;
}

static bool should_export_library(const filesystem::path& path)
{
  static const string export_root_path = "/usr/local/";
  return path.string().substr(0, export_root_path.length()) == export_root_path;
}

static void find_exported_libraries(const filesystem::path& binary, vector<filesystem::path>& result)
{
  vector<filesystem::path> list = find_imported_libraries(binary);

  std::for_each(list.begin(), list.end(), [&result](const filesystem::path file)
  {
    if (should_export_library(file) && std::find(result.begin(), result.end(), file) == result.end())
      result.push_back(file);
  });
}

static vector<filesystem::path> find_application_binaries(const filesystem::path& directory)
{
  filesystem::recursive_directory_iterator dir(directory);
  vector<filesystem::path> results;

  for (auto& entry : dir)
  {
    if (filesystem::is_regular_file(entry.path()))
    {
      auto permissions = filesystem::status(entry.path()).permissions();

      if ((permissions & filesystem::perms::owner_exec) != filesystem::perms::none)
        results.push_back(entry.path());
    }
  }
  return results;
}

static string remove_quotes(const string& source)
{
  if (source[0] == '"' && source[source.length() - 1] == '"')
  {
    string result;
    result = source.substr(1, source.length() - 2);
    return result;
  }
  return source;
}

bool Package::generate_scripts()
{
  FileRenderer renderer;
  vector<std::string> scripts{"start.sh", "stop.sh"};

  renderer.should_overwrite = true;
  renderer.vars["application_name"] = options.count("name") ? options["name"].as<string>() : configuration.variable("name");
  renderer.vars["application_port"] = options.count("port") ? options["port"].as<unsigned short>() : static_cast<unsigned short>(80);
  renderer.vars["bin_directory"]    = bin_target();
  renderer.vars["share_directory"]  = share_target();
  renderer.vars["lib_directory"]    = lib_target();
  renderer.generate_file("package/start.sh", ".tmp/start.sh");
  renderer.generate_file("package/stop.sh",  ".tmp/stop.sh");
  for (const string& script : scripts)
  {
    Crails::run_command("chmod +x .tmp/" + script);
    package_files.push_back(".tmp/" + script);
  }
  return true;
}

static std::string relative_path(const std::string& path)
{
  string result;
  string::const_iterator it = ++path.begin();

  copy(it, path.end(), back_inserter(result));
  return result;
}

bool Package::generate_tarball()
{
  string output = options.count("output") ? options["output"].as<string>() : string("package.tar.gz");
  string tar_command = Crails::which("tar");
  stringstream command;
  string bin_path, lib_path, share_path;

  bin_path = relative_path(bin_target());
  lib_path = relative_path(lib_target());
  share_path = relative_path(share_target());
  if (tar_command.length() > 0)
  {
    command << tar_command << " czf \"" << output << '"'
      << " --dereference"
      << " --transform \"s|usr/local/lib|" << lib_path << "|\""
      << " --transform \"s|build|" << bin_path << "|\""
      << " --transform \"s|.tmp|" << bin_path << "|\""
      << " --transform \"s|public|" << share_path << "/public|\"";
    for (const auto& file : package_files)
      command << ' ' << file;
    command << " public";
    if (options.count("verbose"))
      cout << "+ " << command.str() << endl;
    return Crails::run_command(command.str()) ? 0 : 1;
  }
  else
    cerr << "Could not find `tar`. Perhaps it is not installed ?" << endl;
  return false;
}

void Package::cleanup_tmp()
{
  if (options.count("verbose"))
    cout << "+ rm -rf .tmp" << endl;
  Crails::run_command("rm -rf .tmp");
}

int Package::run()
{
  int result = build_command(configuration, options) ? 0 : -10;

  if (!run_tests(configuration, options))
    return -11;
  if (options.count("install-root"))
    install_directory = options["install-root"].as<string>();
  if (result == 0)
  {
    CleanupFunctor cleanup(std::bind(&Package::cleanup_tmp, this));
    vector<filesystem::path> application_binaries;
    vector<filesystem::path> exported_libraries;
    string output = options.count("output") ? options["output"].as<string>() : string("package.tar.gz");
    string tar_command = Crails::which("tar");
    stringstream command;

    application_binaries = find_application_binaries("build/");
    for (const auto& binary : application_binaries)
      find_exported_libraries(binary, exported_libraries);
    copy(application_binaries.begin(),  application_binaries.end(), back_inserter(package_files));
    copy(exported_libraries.begin(),    exported_libraries.end(),   back_inserter(package_files));
    if (generate_scripts() && generate_tarball())
      return true;
  }
  return result;
}
