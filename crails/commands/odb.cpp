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
    ("input-dirs,i",  boost::program_options::value<std::string>(), "input directories, separated by commas")
    ("verbose,v", "verbose mode (disabled by default)");
}

static bool create_directory(boost::filesystem::path path)
{
  boost::system::error_code ec;

  boost::filesystem::create_directories(path, ec);
  if (ec)
  {
    cerr << "Cannot create directory " << path.string() << endl;
    return false;
  }
  return true;
}

bool BuildOdb::increment_schema_version()
{
  string odb_hpp;
  Crails::RenderFile odb_hpp_output;

  if (Crails::read_file("config/odb.hpp", odb_hpp))
  {
    regex version_pattern("^#pragma db model version\\([0-9]+,([0-9]+)\\)$", std::regex_constants::multiline);
    auto match = sregex_iterator(odb_hpp.begin(), odb_hpp.end(), version_pattern);
    if (match != sregex_iterator() && match->size() > 1)
    {
      unsigned int current_version = boost::lexical_cast<unsigned int>(odb_hpp.substr(match->position(1), match->length(1)));
      odb_hpp.erase(match->position(1), match->length(1));
      odb_hpp.insert(match->position(1), boost::lexical_cast<string>(current_version + 1));
      if (odb_hpp_output.open("config/odb.hpp"))
      {
        odb_hpp_output.set_body(odb_hpp.c_str(), odb_hpp.length());
        return true;
      }
      else
        cerr << "Cannot write into config/odb.hpp" << endl;
    }
    else
      cerr << "Cannot find #pragma db model version in odb.hpp" << endl;
  }
  else
    cerr << "Cannot open config/odb.hpp" << endl;
  return false;
}

int BuildOdb::run()
{
  FileList files;

  temporary_dir = ".tmp-odb";
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
    if (create_directory(output_dir)
     && create_directory(temporary_dir)
     && compile_models(files))
    {
      apply_new_version();
      return 0;
    }
    return -1;
  }
  else if (options.count("verbose"))
    cout << "[crails-odb] Nothing to compile." << endl;
  return 0;
}

bool BuildOdb::compile_models(const FileList& files)
{
  if (at_once)
    return compile_models_at_once(files) && generate_schema(files);
  return compile_models_one_by_one(files) && generate_schema(files);
}

bool BuildOdb::compile_models_at_once(const FileList& files)
{
  boost::filesystem::path project_dir = boost::filesystem::canonical(configuration.project_directory());
  stringstream command;

  command << odb_command(temporary_dir) << ' ';
  for (auto file : files)
    command << boost::filesystem::relative(file, project_dir) << ' ';
  if (options.count("verbose"))
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
  string source_path_base = temporary_dir + '/' + input_name;
  vector<string> targets;
  string source_path = temporary_dir + '/' + input_name + "-odb.hxx";
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
    string command = odb_command(temporary_dir)
      + ' ' + "--include-prefix \"" + include_path.string() + '"'
      + ' ' + file.string();
    if (options.count("verbose"))
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
  if (increment_schema_version())
  {
    boost::filesystem::path project_dir = boost::filesystem::canonical(configuration.project_directory());
    stringstream command;
    bool   embed_schema      = configuration.variable("odb-embed-schema") == "1";
    string schema_output_dir = embed_schema ? output_dir : string("tasks/odb_migrate");

    at_once = true;
    command << odb_command(schema_output_dir) << ' ' << "--generate-schema-only" << ' ';
    for (auto file : files)
      command << boost::filesystem::relative(file, project_dir) << ' ';
    if (options.count("verbose"))
      cout << "+ " << command.str() << endl;
    boost::process::child odb(command.str());
    odb.wait();
    if (odb.exit_code() == 0)
      return true;
  }
  return false;
}

BuildOdb::FileList BuildOdb::collect_files()
{
  FileList results;
  regex pattern("#\\s*pragma\\s+db\\s+object", regex_constants::ECMAScript);
  bool verbose = options.count("verbose");

  for (const string& input_dir : input_dirs)
  {
    if (!boost::filesystem::is_directory(input_dir)) continue ;
    FileCollector(input_dir, "\\.h(pp|xx)?$").collect_files([&results, pattern, verbose](const boost::filesystem::path& path)
    {
      string file_contents;

      if (Crails::read_file(path.string(), file_contents) && sregex_iterator(file_contents.begin(), file_contents.end(), pattern) != sregex_iterator())
      {
        if (verbose)
           cout << "[crails-odb] detected odb header " << path.string() << endl;
        results.push_back(path);
      }
      else if (verbose)
        cout << "[crails-odb] rejected potential odb header " << path.string() << endl;
    });
  }
  return results;
}

void BuildOdb::apply_new_version()
{
  FileCollector collector(temporary_dir, ".*");
  FileCollector clearer(output_dir, ".*");
  vector<string> existing_files;

  collector.collect_files([&existing_files, this](const boost::filesystem::path& path)
  {
    boost::filesystem::path old_path(output_dir + '/' + path.filename().string());
    string new_contents, old_contents;

    existing_files.push_back(path.filename().string());
    Crails::read_file(path.string(), new_contents);
    Crails::read_file(old_path.string(), old_contents);
    if (new_contents != old_contents)
      boost::filesystem::copy(path, old_path, boost::filesystem::copy_options::overwrite_existing);
  });
  clearer.collect_files([&existing_files](const boost::filesystem::path& path)
  {
    if (std::find(existing_files.begin(), existing_files.end(), path.filename().string()) == existing_files.end())
      boost::filesystem::remove(path);
  });
}

string BuildOdb::odb_command(const string& output_dir)
{
  stringstream stream;

  stream << odb_compiler
         << " -I."
         << " --std " << configuration.variable("std")
         << " --schema-format separate"
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
