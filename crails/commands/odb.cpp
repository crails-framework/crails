#include "odb.hpp"
#include "../file_collector.hpp"
#include <crails/read_file.hpp>
#include <crails/utils/string.hpp>
#include <boost/process.hpp>
#include <iostream>

using namespace std;

void BuildOdb::options_description(boost::program_options::options_description& desc) const
{
  desc.add_options()
    ("at-once,a",     boost::program_options::value<bool>(),        "odb --at-once")
    ("includes,b",    boost::program_options::value<std::string>(), "custom includes, separated by commas")
    ("use-session,c", boost::program_options::value<bool>(),        "use odb session")
    ("output-dir,o",  boost::program_options::value<std::string>(), "output directory")
    ("input-dirs,i",  boost::program_options::value<std::string>(), "input directories, separated by commas");
}

int BuildOdb::run()
{
  FileList files;

  backends = Crails::split(configuration.variable("odb-backends"), ',');
  if (options.count("at-once"))
    at_once = options["at-once"].as<bool>();
  if (options.count("use-session"))
    use_session = options["use-session"].as<bool>();
  if (options.count("includes"))
    custom_includes = Crails::split(options["includes"].as<string>(), ',');
  output_dir = options.count("output-dir") ? options["output-dir"].as<string>() : string("lib");
  input_dirs = options.count("input-dirs") ? Crails::split(options["input-dirs"].as<string>(), ',') : list<string>{"app/models"};
  files = collect_files();
  if (files.size() > 0)
  {
    if (compile_models(files) && generate_schema(files))
      return 0;
    return -1;
  }
  else
    cout << "[crails-odb] Nothing to compile." << endl;
  return 0;
}

bool BuildOdb::compile_models(const FileList& files)
{
  for (auto file : files)
  {
    string command = odb_command() + ' ' + file.string();
    cout << "+ " << command << endl;
    boost::process::child odb(command);

    odb.wait();
    if (odb.exit_code() != 0)
      return false;
  }
  return true;
}

bool BuildOdb::generate_schema(const FileList& files)
{
  return true;
}

BuildOdb::FileList BuildOdb::collect_files()
{
  FileList results;
  regex pattern("#\\s*pragma\\s+db\\s+object", regex_constants::ECMAScript);

  std::cout << "BuildOdb::collect_files in dirs: " << input_dirs.size() << " dirs" << std::endl;
  for (const string& input_dir : input_dirs)
  {
    FileCollector collector(input_dir, "\\.h(pp|xx)?$");

    std::cout << "Collecting file in dir " << input_dir << std::endl;
    collector.collect_files([&results, pattern](const boost::filesystem::path& path)
    {
      string file_contents;

      if (Crails::read_file(path.string(), file_contents) && sregex_iterator(file_contents.begin(), file_contents.end(), pattern) != sregex_iterator())
      {
        cout << "[crails-odb] detected odb header " << path.string() << endl;
        results.push_back(path);
      }
      else
        cout << "[crails-odb] rejected potential odb header " << path.string() << endl;
    });
  }
  return results;
}

string BuildOdb::odb_command()
{
  stringstream stream;

  stream << odb_compiler
         << " -I."
         << " --schema-format separate"
         << " --output-dir " << output_dir
         << " --hxx-prologue '" << hxx_prologue() << '\''
         << " --output-dir '" << output_dir << '\'';
  if (default_pointer.length() > 0)
    stream << " --default-pointer " << default_pointer;
  if (table_prefix.length() > 0)
    stream << " --table-prefix '" << table_prefix << '\'';
  if (at_once)
    stream << " --at-once --input-name " << input_name;
  for (const string& backend : backends)
    stream << " -d " << backend;
  if (backends.size() > 1)
    stream << " --multi-database dynamic -d common";
  if (use_session)
    stream << " --generate-session";
  stream << " --generate-query";
  return stream.str();
}

string BuildOdb::hxx_prologue()
{
  stringstream prologue;

  for (const string& include : custom_includes)
    prologue << "#include \"" << include << '"' << std::endl;
  prologue << hxx_prologue_;
  return prologue.str();
}
