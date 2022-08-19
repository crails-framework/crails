#include "odb.hpp"
#include "../file_collector.hpp"
#include <crails/read_file.hpp>
#include <crails/utils/string.hpp>
#include <crails/render_file.hpp>
#include <boost/process.hpp>
#include <iostream>

using namespace std;

void BuildOdb::options_description(boost::program_options::options_description& desc) const
{
  desc.add_options()
    ("includes,b",    boost::program_options::value<std::string>(), "custom includes, separated by commas")
    ("use-session,c", boost::program_options::value<bool>(),        "use odb session")
    ("output-dir,o",  boost::program_options::value<std::string>(), "output directory")
    ("input-dirs,i",  boost::program_options::value<std::string>(), "input directories, separated by commas");
}

int BuildOdb::run()
{
  FileList files;

  backends = Crails::split(configuration.variable("odb-backends"), ',');
  at_once = configuration.variable("odb-at-once") == "1";
  default_pointer = configuration.variable("odb-default-pointer");
  if (options.count("use-session"))
    use_session = options["use-session"].as<bool>();
  if (options.count("includes"))
    custom_includes = Crails::split(options["includes"].as<string>(), ',');
  output_dir = options.count("output-dir") ? options["output-dir"].as<string>() : string("lib/odb");
  input_dirs = options.count("input-dirs") ? Crails::split(options["input-dirs"].as<string>(), ',') : list<string>{"app/models"};
  files = collect_files();
  if (files.size() > 0)
  {
    boost::system::error_code ec;

    boost::filesystem::create_directories(output_dir, ec);
    if (ec)
    {
      cerr << "Cannot create directory " << output_dir << endl;
      return -1;
    }
    return compile_models(files) ? 0 : -1;
  }
  else
    cout << "[crails-odb] Nothing to compile." << endl;
  return 0;
}

bool BuildOdb::compile_models(const FileList& files)
{
  if (at_once)
    return compile_models_at_once(files);
  return compile_models_one_by_one(files) && generate_schema(files);
}

bool BuildOdb::compile_models_at_once(const FileList& files)
{
  boost::filesystem::path project_dir = boost::filesystem::canonical(configuration.project_directory());
  stringstream command;

  command << odb_command() << ' ';
  for (auto file : files)
    command << boost::filesystem::relative(file, project_dir) << ' ';
  cout << "+ " << command.str() << endl;
  boost::process::child odb(command.str());
  odb.wait();
  if (odb.exit_code() != 0)
    return false;
  return at_once_fix_include_paths(files);
}

bool BuildOdb::at_once_fix_include_paths(const FileList& files) const
{
  Crails::RenderFile render_file;
  boost::filesystem::path project_dir = boost::filesystem::canonical(configuration.project_directory());
  string source_path_base = output_dir + '/' + input_name;
  vector<string> targets;
  string source_path = output_dir + '/' + input_name + "-odb.hxx";
  string source;

  targets.push_back(source_path_base + "-odb.hxx");
  if (backends.size() > 1)
  {
    for (const string& backend : backends)
      targets.push_back(source_path_base + '-' + backend + "-odb.hxx");
  }
  for (const string& source_path : targets)
  {
    if (Crails::read_file(source_path, source))
    {
      list<string> lines = Crails::split(source, '\n');
      list<string> result;
      bool within_local_includes = false;

      for (auto it = lines.begin() ; it != lines.end() ; ++it)
      {
        if (within_local_includes)
        {
          if (*it == "#include <memory>")
          {
            within_local_includes = false;
            result.push_back(*it);
          }
        }
        else if (*it == "#include <odb/pre.hxx>")
        {
          within_local_includes = true;
          result.push_back(*it);
          for (const auto& filepath : files)
            result.push_back("#include \"" + boost::filesystem::relative(filepath, project_dir).string() + '"');
        }
        else
          result.push_back(*it);
      }
      source = Crails::join(result, '\n');
      render_file.open(source_path);
      render_file.set_body(source.c_str(), source.length());
    }
    else
    {
      cerr << "Could not open " << source_path << " for reading" << endl;
      return false;
    }
  }
  return true;
}

bool BuildOdb::compile_models_one_by_one(const FileList& files)
{
  boost::filesystem::path project_dir = boost::filesystem::canonical(configuration.project_directory());

  for (auto file : files)
  {
    boost::filesystem::path include_path = boost::filesystem::relative(file, project_dir).parent_path();
    string command = odb_command()
      + ' ' + "--include-prefix \"" + include_path.string() + '"'
      + ' ' + file.string();
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

    if (!boost::filesystem::is_directory(input_dir)) continue ;
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
         << " --std " << configuration.variable("std")
         << " --schema-format separate"
         << " --output-dir " << output_dir
         << " --hxx-prologue \"" << hxx_prologue() << '"'
         << " --output-dir " << output_dir;
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

  prologue << "#include <crails/safe_ptr.hpp>\n";
  for (const string& include : custom_includes)
    prologue << "#include \\\"" << include << "\\\"\n";
  prologue << hxx_prologue_;
  return prologue.str();
}
