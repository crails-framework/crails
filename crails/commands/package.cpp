#include "package.hpp"
#include <crails/cli/process.hpp>
#include <crails/utils/split.hpp>
#include <boost/process.hpp>
#include <filesystem>
#include <iostream>

using namespace std;

static int build_command(const ProjectConfiguration& configuration, boost::program_options::variables_map& options)
{
  stringstream command;

  command << configuration.crails_bin_path() << "/crails build";
  if (options.count("verbose"))
    command << " -v";
  if (options.count("mode"))
    command << " -m " << options["mode"].as<string>();
  if (options.count("verbose"))
    cout << "+ " << command.str() << endl;
  return Crails::run_command(command.str());
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

static vector<filesystem::path> find_exported_libraries(const filesystem::path& binary)
{
  vector<filesystem::path> list = find_imported_libraries(binary);
  const string export_root_path = "/usr/local/";

  std::remove_if(list.begin(), list.end(), [&export_root_path](const filesystem::path& path)
  {
    return path.string().substr(0, export_root_path.length()) != export_root_path;
  });
  return list;
}

static vector<filesystem::path> find_application_binaries(const filesystem::path& directory)
{
  filesystem::recursive_directory_iterator dir(directory);
  vector<filesystem::path> results;

  for (auto& entry : dir)
  {
    auto permissions = filesystem::status(entry.path()).permissions();

    if ((permissions & filesystem::perms::owner_exec) != filesystem::perms::none)
      results.push_back(entry.path());
  }
  return results;
}

int Package::run()
{
  int result = build_command(configuration, options);

  if (result == 0)
  {
    auto exported_libraries = find_exported_libraries("build/server");
    auto application_binaries = find_application_binaries("build/");
    string output = options.count("output") ? options["output"].as<string>() : string("package.tar.gz");
    string tar_command = Crails::which("tar");
    stringstream command;

    if (tar_command.length() > 0)
    {
      command << tar_command << " czf \"" << output << '"'
        << " --transform=s/usr\\\\/local\\\\///"
        << " --transform=s/build/bin/";
      for (const auto& binary : application_binaries)
        command << ' ' << binary;
      for (const auto& library : exported_libraries)
        command << ' ' << library;
      command << " public";
      if (options.count("verbose"))
        cout << "+ " << command.str() << endl;
      return Crails::run_command(command.str()) ? 0 : 1;
    }
    else
      cerr << "Could not find `tar`. Perhaps it is not installed ?" << endl;
    return 0;
  }
  return result;
}
