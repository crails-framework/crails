#include "odb.hpp"
#include "../file_collector.hpp"
#include <crails/read_file.hpp>
#include <crails/utils/string.hpp>
#include <crails/render_file.hpp>
#include <crails/cli/process.hpp>
#include <boost/process.hpp>
#include <iostream>
#include <filesystem>

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

static bool create_directory(filesystem::path path)
{
  error_code ec;

  filesystem::create_directories(path, ec);
  if (ec)
  {
    cerr << "Cannot create directory " << path.string() << endl;
    return false;
  }
  return true;
}

static bool is_path_to_schema(const filesystem::path& path)
{
  string filename = path.filename().string();

  return filename.compare(filename.length() - 11, 11, "-schema.cxx") == 0;
}

std::string application_xml_path()
{
  return "app/config/odb.xml";
}

void BuildOdb::clear_empty_changesets()
{
#ifndef _MSC_VER
  string application_xml;
  Crails::RenderFile application_xml_output;

  if (Crails::read_file(application_xml_path(), application_xml))
  {
    regex  empty_changeset_pattern("^\\s*<changeset\\s+version=\"[0-9]+\"\\s*/>\\s*$",  regex_constants::multiline);
    auto   match = sregex_iterator(application_xml.begin(), application_xml.end(), empty_changeset_pattern);
    int    last_it = 0;
    string output;

    while (match != sregex_iterator())
    {
      output += application_xml.substr(last_it, match->position() - last_it);
      last_it = match->position() + match->length();
      match++;
    }
    if (last_it > 0)
    {
      output += application_xml.substr(last_it);
      if (application_xml_output.open(application_xml_path()))
        application_xml_output.set_body(output.c_str(), output.length());
      else
        cerr << "[crails-odb] /!\\ WARNING: canont open " << application_xml_path() << " for writing." << endl;
    }
  }
  else
    cerr << "[crails-odb] /!\\ WARNING: cannot open " << application_xml_path() << endl;
#else
  cerr << "[crails-odb] /!\\ crails cli was built with MSVC and cannot clear empty changesets from " << application_xml_path() << endl;
#endif
}

bool BuildOdb::increment_schema_version()
{
#ifndef _MSC_VER
  string odb_hpp;
  string application_xml;
  Crails::RenderFile odb_hpp_output;

  Crails::read_file(application_xml_path(), application_xml);
  if (Crails::read_file("app/config/odb.hpp", odb_hpp))
  {
    regex version_pattern("^#pragma db model version\\([0-9]+,([0-9]+)\\)$", std::regex_constants::multiline);
    auto match = sregex_iterator(odb_hpp.begin(), odb_hpp.end(), version_pattern);

    if (match != sregex_iterator() && match->size() > 1)
    {
      unsigned int current_version = boost::lexical_cast<unsigned int>(odb_hpp.substr(match->position(1), match->length(1)));
      auto         has_changeset   = application_xml.find("<changeset version=\"" + std::to_string(current_version) + "\">");

      if (has_changeset == std::string::npos)
        return true;
      else
      {
        cout << "[crails-odb] Pattern not found in " << application_xml_path() << ": " << "<changeset version=\"" << std::to_string(current_version) << "\">" << endl;
        cout << "[crails-odb] Incrementing schema version" << endl;
      }
      odb_hpp.erase(match->position(1), match->length(1));
      odb_hpp.insert(match->position(1), boost::lexical_cast<string>(current_version + 1));
      if (odb_hpp_output.open("app/config/odb.hpp"))
      {
        odb_hpp_output.set_body(odb_hpp.c_str(), odb_hpp.length());
        return true;
      }
      else
        cerr << "Cannot write into app/config/odb.hpp" << endl;
    }
    else
      cerr << "Cannot find #pragma db model version in odb.hpp" << endl;
  }
  else
    cerr << "Cannot open app/config/odb.hpp" << endl;
  return false;
#else
  cerr << "[crails-odb] /!\\ crails cli was built with MSVC and cannot increment schema version in `app/config/odb.hpp`. Please perform the update yourself." << endl;
  return true;
#endif
}

static list<string> default_input_directories(const ProjectConfiguration& configuration)
{
  list<string> base{"app/models"};

  for (const string& module_ : configuration.modules())
    base.push_back("modules/" + module_ + "/models");
  return base;
}

int BuildOdb::run()
{
  FileList files;
  odb_compiler = Crails::which("odb");

  if (odb_compiler.length() == 0)
  {
    cerr << "[crails-odb] /!\\ odb compiler not found." << endl;
    return -2;
  }
  temporary_dir = ".tmp-odb";
  backends = Crails::split(configuration.variable("odb-backends"), ',');
  at_once = configuration.variable("odb-at-once") == "1";
  default_pointer = configuration.variable("odb-default-pointer");
  if (options.count("use-session"))
    use_session = options["use-session"].as<bool>();
  if (options.count("includes"))
    custom_includes = Crails::split(options["includes"].as<string>(), ',');
  output_dir = options.count("output-dir") ? options["output-dir"].as<string>() : (configuration.autogen_path() / "odb").string();
  input_dirs = options.count("input-dirs") ? Crails::split(options["input-dirs"].as<string>(), ',') : default_input_directories(configuration);
  files = collect_files();
  if (files.size() > 0)
  {
    if (create_directory(output_dir)
     && create_directory(temporary_dir)
     && compile_models(files))
    {
      clear_empty_changesets();
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
  std::filesystem::path project_dir = filesystem::canonical(configuration.project_directory());
  stringstream command;

  command << odb_command(temporary_dir) << ' ';
  for (auto file : files)
    command << filesystem::relative(file, project_dir) << ' ';
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
  string header_ext = configuration.source_extension(HeaderExt);
  filesystem::path project_dir = filesystem::canonical(configuration.project_directory());
  string source_path_base = temporary_dir + '/' + input_name;
  vector<string> targets;
  string source_path = temporary_dir + '/' + input_name + "-odb." + header_ext;
  string source;

  targets.push_back(source_path_base + "-odb." + header_ext);
  if (backends.size() > 1)
  {
    for (const string& backend : backends)
      targets.push_back(source_path_base + '-' + backend + "-odb." + header_ext);
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
            result.push_back("#include \"" + filesystem::relative(filepath, project_dir).string() + '"');
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
  filesystem::path project_dir = filesystem::canonical(configuration.project_directory());

  for (auto file : files)
  {
    filesystem::path include_path = filesystem::relative(file, project_dir).parent_path();
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
    filesystem::path project_dir = filesystem::canonical(configuration.project_directory());
    filesystem::path config_odb_hpp = project_dir / "app" / "config" / "odb.hpp";
    stringstream command;
    bool   embed_schema      = configuration.variable("odb-embed-schema") == "1";
    string schema_output_dir = embed_schema ? output_dir : string("exe/odb_migrate");

    at_once = true;
    command << odb_command(schema_output_dir) << ' '
            << "--generate-schema-only" << ' ';
    if (filesystem::exists(config_odb_hpp))
      command << filesystem::relative(config_odb_hpp, project_dir) << ' ';
    else
      cerr << "Could not find default ODB configuration file at " << config_odb_hpp << ". The database schema likely won't be versioned and the odb_migrate task will only be able to create databases from scratch." << endl; 
    for (auto file : files)
      command << filesystem::relative(file, project_dir) << ' ';
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
  regex pattern("#\\s*pragma\\s+db\\s+(view|object)", regex_constants::ECMAScript);
  bool verbose = options.count("verbose");

  for (const string& input_dir : input_dirs)
  {
    if (!filesystem::is_directory(input_dir)) continue ;
    FileCollector(input_dir, "\\.h(pp|xx)?$").collect_files([&results, pattern, verbose](const filesystem::path& path)
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

  collector.collect_files([&existing_files, this](const filesystem::path& path)
  {
    filesystem::path old_path(output_dir + '/' + path.filename().string());
    string new_contents, old_contents;

    existing_files.push_back(path.filename().string());
    Crails::read_file(path.string(), new_contents);
    Crails::read_file(old_path.string(), old_contents);
    if (new_contents != old_contents)
      filesystem::copy(path, old_path, filesystem::copy_options::overwrite_existing);
  });
  clearer.collect_files([&existing_files](const filesystem::path& path)
  {
    if (is_path_to_schema(path))
      return ;
    if (std::find(existing_files.begin(), existing_files.end(), path.filename().string()) == existing_files.end())
      filesystem::remove(path);
  });
}

string BuildOdb::odb_command(const string& output_dir)
{
  stringstream stream;

  stream << odb_compiler
         << " -I." << " -I./app"
         << " --std " << configuration.variable("std")
         << " --hxx-suffix ." << configuration.source_extension(HeaderExt)
         << " --ixx-suffix ." << configuration.source_extension(InlineExt)
         << " --cxx-suffix ." << configuration.source_extension(SourceExt)
         << " --schema-format separate"
         << " --hxx-prologue \"" << hxx_prologue() << '"'
         << " --output-dir " << output_dir;
  stream << " --changelog \"" << application_xml_path() << '"';
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
    prologue << "#include \\\"" << include << "\\\"\n";
  prologue << hxx_prologue_;
  return prologue.str();
}
