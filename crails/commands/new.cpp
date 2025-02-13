#include "new.hpp"
#include <boost/process.hpp>
#include <boost/program_options.hpp>
#include <crails/utils/string.hpp>
#include <crails/cli/conventions.hpp>
#include <crails/cli/process.hpp>
#include <regex>
#include <filesystem>
#include <iostream>
#include "../version.hpp"

using namespace std;

const vector<string> odb_backends{"sqlite","pgsql","mysql","oracle"};
const vector<string> db_backends{"sqlite","pgsql","mysql","oracle","mongodb"};

New::New() : vars(renderer.vars)
{
  with_configuration = false;
  vars["crails_version"] = string(LIBCRAILS_VERSION_STR);
  vars["renderers"] = &project_renderers;
  vars["parsers"]   = &project_parsers;
  vars["handlers"]  = &project_handlers;
  vars["configuration"] = &configuration;
}

void New::options_description(boost::program_options::options_description& desc) const
{
  desc.add_options()
    ("name,n",          boost::program_options::value<string>(), "project name")
    ("target,o",        boost::program_options::value<string>(), "folder in which the project will be generated (equal to `name` by default)")
    ("build-dir,b",     boost::program_options::value<string>(), "build directory, relative to the project root (build/ by default)")
    ("toolchain,t",     boost::program_options::value<string>(), "options: cmake or build2 (defaults to cmake)")
    ("configuration,c", boost::program_options::value<string>(), "options: full or webservice (defaults to full)")
    ("database,d",      boost::program_options::value<string>(), "options: sqlite, pgsql, mysql, oracle, mongodb")
    ("session-store",   boost::program_options::value<string>(), "options: NoSessionStore, CookieStore")
    ("formats,p",       boost::program_options::value<string>(), "options: html,json,xml")
    ("force,f", "overwrite existing files without asking");
}

void New::generate_cmake_files()
{
  generate_file("CMakeLists.txt");
  generate_file(".gitignore");
  renderer.generate_file("spec/CMakeLists.txt", "spec/driver/CMakeLists.txt");
}

void New::generate_build2_files()
{
  bool should_overwrite_backup = renderer.should_overwrite;

  renderer.should_overwrite = true; // bdep-new will have generated the following files 
  generate_file(".gitignore");
  renderer.generate_file("build2/buildfile",              "buildfile");
  renderer.generate_file("build2/manifest",               "manifest");
  renderer.generate_file("build2/repositories.manifest",  "repositories.manifest");
  renderer.generate_file("build2/build/bootstrap.build",  "build/bootstrap.build");
  renderer.generate_file("build2/build/export.build",     "build/export.build");
  renderer.generate_file("build2/build/root.build",       "build/root.build");
  renderer.should_overwrite = should_overwrite_backup;
  renderer.generate_file("build2/app/buildfile",          "app/buildfile");
  renderer.generate_file("build2/exe/buildfile",          "exe/buildfile");
  renderer.generate_file("build2/exe/bootstrap.build",    "exe/build/bootstrap.build");
  renderer.generate_file("build2/exe/root.build",         "exe/build/root.build");
  renderer.generate_file("build2/server/buildfile",       "exe/server/buildfile");
  renderer.generate_file("build2/exe/buildfile",          "spec/buildfile");
  renderer.generate_file("build2/spec/bootstrap.build",   "spec/build/bootstrap.build");
  renderer.generate_file("build2/spec/root.build",        "spec/build/root.build");
  renderer.generate_file("build2/spec/buildfile",         "spec/driver/buildfile");
}

bool New::generate_project_structure()
{
  auto renderers = configuration.renderers();

  if (build_system == "build2")
    generate_build2_files();
  else
    generate_cmake_files();
  renderer.generate_file("app/main.cpp", "exe/server/main.cpp");
  renderer.generate_file("config/server.cpp", "app/config/server.cpp");
  renderer.generate_file("config/server.hpp", "app/config/server.hpp");
  renderer.generate_file("config/environment.cpp", "app/config/environment.cpp");
  renderer.generate_file("config/renderers.hpp", "app/config/renderers.hpp");
  renderer.generate_file("config/renderers.cpp", "app/config/renderers.cpp");
  renderer.generate_file("config/request_pipe.cpp", "app/config/request_pipe.cpp");
  renderer.generate_file("config/session_store.hpp", "app/config/session_store.hpp");
  renderer.generate_file("config/session_store.cpp", "app/config/session_store.cpp");
  generate_file("public/index.html");
  renderer.generate_file("spec/main.cpp", "spec/driver/main.cpp");
  if (configuration.has_plugin("libcrails-action"))
  {
    generate_file("app/routes.cpp");
    renderer.generate_file("config/router.hpp", "app/config/router.hpp");
  }
  if (configuration.has_plugin("libcrails-controllers"))
    generate_file("app/controllers/application.hpp");
  if (find(renderers.begin(), renderers.end(), "html") != renderers.end())
    generate_file("app/views/exception.html");
  if (find(renderers.begin(), renderers.end(), "json") != renderers.end())
    generate_file("app/views/exception.json");
  configuration.save();
  return true;
}

bool New::generate_database(const string& backend)
{
  renderer.generate_file("config/databases.hpp", "app/config/databases.hpp");
  renderer.generate_file("config/databases.cpp", "app/config/databases.cpp");
  if (find(odb_backends.begin(), odb_backends.end(), backend) != odb_backends.end())
  {
    boost::process::child command(configuration.crails_bin_path() + "/crails plugins odb install -b " + backend);

    command.wait();
    return command.exit_code() == 0;
  }
  else if (backend == "mongodb")
  {
    boost::process::child command(configuration.crails_bin_path() + "/crails plugins mongodb install -b " + backend);

    command.wait();
    return command.exit_code() == 0;
  }
  return true;
}

int New::run()
{
  if (options.count("name"))
  {
    project_name = options["name"].as<string>();
    if (options.count("toolchain"))
      build_system = options["toolchain"].as<string>();
    if (options.count("configuration"))
      configuration_type = options["configuration"].as<string>();
    if (options.count("formats"))
      formats = Crails::split(options["formats"].as<string>(), ',');
    if (options.count("force") != 0)
      renderer.should_overwrite = true;
    if (options.count("session-store"))
      session_store = Crails::camelize(options["session-store"].as<string>());
    else if (configuration_type == "full")
      session_store = string("CookieStore");
    vars["session_store"] = session_store;
    if (validate_options() && move_to_project_directory())
    {
      configuration.version(LIBCRAILS_VERSION_STR);
      configuration.toolchain(build_system);
      configuration.asset_roots({"app/assets", "app/autogen/assets"});
      configuration.add_plugin("libcrails");
      configuration.add_plugin("libcrails-logger");
      configuration.variable("std", "c++17");
      configuration.variable("name", project_name);
      vars["project_name"] = project_name;
      vars["crails_version"] = configuration.version();
      vars["configuration_type"] = configuration_type;
      vars["build_system"] = build_system;
      vars["formats"] = &formats;
      vars["plugins"] = &plugins;
      vars["cpp_version"] = configuration.variable("std");
      use_actions(configuration_type == "full" || configuration_type == "webservice");
      use_cookies(Crails::cast<string>(vars, "session_store", "NoCookieStore") != "NoCookieStore");
      use_databases(options.count("database"));
      prepare_renderers();
      prepare_request_pipeline();
      plugins = configuration.plugins();
      if (generate_project_structure())
      {
        if (options.count("database"))
          generate_database(options["database"].as<string>());
        return 0;
      }
    }
  }
  return -1;
}

bool New::validate_options()
{
  vector<string> available_configuration_types{"full","webservice","barebone"};
  vector<string> available_toolchains{"cmake", "build2"};
  regex validate_name("^[a-z0-9_\\-+\\.]{2}[a-z0-9_\\-+\\.]*$", regex_constants::icase);

  if (sregex_iterator(project_name.begin(), project_name.end(), validate_name) == sregex_iterator())
  {
    cerr << "Invalid project name." << endl;
    return false;
  }
  if (find(available_configuration_types.begin(), available_configuration_types.end(), configuration_type) == available_configuration_types.end())
  {
    cerr << "Invalid configuration `" << configuration_type << "`. Options are: " << Crails::join(available_configuration_types, ',') << '.' << endl;
    return false;
  }
  if (find(available_toolchains.begin(), available_toolchains.end(), build_system) == available_toolchains.end())
  {
    cerr << "Invalid toolchain `" << build_system << "`. Options are: " << Crails::join(available_toolchains, ',') << '.' << endl;
    return false;
  }
  if (options.count("database"))
  {
    string database = options["database"].as<string>();
    if (database != "none" && find(db_backends.begin(), db_backends.end(), database) == db_backends.end())
    {
      cerr << "Unknown database type `" << database << "`. Options are: " << Crails::join(db_backends, ',') << '.' << endl;
      return false;
    }
  }
  return true;
}

void New::prepare_renderers()
{
  if (formats.size())
  {
    configuration.add_plugin("libcrails-renderer");
    configuration.add_plugin("libcrails-templates");
  }
  if (find(formats.begin(), formats.end(), "html") != formats.end())
  {
    project_renderers.push_back({
      Crails::naming_convention.filenames(configuration.project_name() + "_html_renderer"),
      Crails::naming_convention.classnames(configuration.project_name() + "_html_renderer")
    });
    configuration.add_renderer("html");
    configuration.add_plugin("libcrails-html-views");
  }
  if (find(formats.begin(), formats.end(), "json") != formats.end())
  {
    project_renderers.push_back({
      Crails::naming_convention.filenames(configuration.project_name() + "_json_renderer"),
      Crails::naming_convention.classnames(configuration.project_name() + "_json_renderer")
    });
    configuration.add_renderer("json");
    configuration.add_plugin("libcrails-json-views");
  }
}

void New::prepare_request_pipeline()
{
  project_handlers.push_back({"file", "FileRequestHandler"});
  project_parsers.push_back({"url_parser", "RequestUrlParser"});
  configuration.add_plugin("libcrails-url-parser");
  if (session_store == "CookieStore")
  {
    configuration.add_plugin("libcrails-encrypt");
    configuration.add_plugin("libcrails-cookies");
  }
  if (configuration_type == "full" || configuration_type == "webservice")
  {
    project_handlers.push_back({"action", "ActionRequestHandler"});
    configuration.add_plugin("libcrails-action");
    configuration.add_plugin("libcrails-controllers");
  }
  if (configuration_type == "full")
  {
    project_parsers.push_back({"form_parser", "RequestFormParser"});
    project_parsers.push_back({"multipart_parser", "RequestMultipartParser"});
    configuration.add_plugin("libcrails-form-parser");
    configuration.add_plugin("libcrails-multipart-parser");
  }
  if (find(formats.begin(), formats.end(), "json") != formats.end())
  {
    project_parsers.push_back({"json_parser", "RequestJsonParser"});
    configuration.add_plugin("libcrails-json-parser");
  }
  if (find(formats.begin(), formats.end(), "xml") != formats.end())
  {
    project_parsers.push_back({"xml_parser", "RequestXmlParser"});
    configuration.add_plugin("libcrails-xml-parser");
  }
}

bool New::move_to_project_directory()
{
  filesystem::path path(target().data());

  if (!filesystem::is_directory(path))
  {
    error_code ec;

    if (build_system == "build2")
    {
      ostringstream command;

      command << "bdep new -l c++ -t bare " << quoted(project_name);
      if (!Crails::run_command(command.str()))
      {
        cerr << "Command `" << command.str() << "` failed." << endl;
        return false;
      }
    }
    else
    {
      filesystem::create_directories(path, ec);
      if (ec)
      {
        cout << "Failed to reach project directory: " << ec.message() << endl;
        return false;
      }
    }
  }
  filesystem::current_path(path);
  return true;
}
