#include "module.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "../../commands/odb.hpp"
#include "../../file_renderer.hpp"
#include "../../file_editor.hpp"
#include "../../build2_editor.hpp"
#include <crails/cli/build2_builder.hpp>

using namespace std;

const vector<string> OdbModule::supported_backends{"sqlite","pgsql","mysql","oracle"};

OdbModule::OdbModule()
{
  add_command("install",  []() { return make_shared<OdbInstaller>(); });
  add_command("disable",  []() { return make_shared<OdbDisabler>(); });
  add_command("backends", []() { return make_shared<OdbBackendManager>(); });
  add_command("build",    []() { return make_shared<BuildOdb>(); });
}

static void add_database(const ProjectConfiguration& configuration, const std::string& backend)
{
  CppFileEditor databases("app/config/databases.cpp", "");
  stringstream  desc;
  const std::map<std::string, unsigned short> ports = {{"mysql", 3306}, {"pgsql", 5432}};

  desc << "      ";
  desc << "{\"odb\",{";
  desc << "{\"type\",\"" << backend << "\"}";
  desc << ",{\"name\", \"" << configuration.variable("name") << "\"}";
  if (backend != "sqlite")
    desc << ",{\"host\",\"127.0.0.1\"}";
  if (ports.find(backend) != ports.end())
    desc << ",{\"port\", static_cast<unsigned int>(" << ports.at(backend) << ")}";
  desc << "}}\n";
  databases.load_file();
  databases.use_symbol("Development, \\{");
  databases.insert(desc.str());
  databases.save_file();
}

void build2_configure_libcrails_odb(Build2Builder& build2, const ProjectConfiguration& configuration)
{
  filesystem::path build_path(configuration.application_build_path());
  const list<string> backends = Crails::split(configuration.variable("odb-backends"), ',');

  cout << "== build2_configure_libcrails_odb" << endl;
  if (backends.size() > 0)
  {
    string package_name = "libcrails-odb-" + configuration.version();
    CrailsFileEditor config_build(
      build_path / package_name / "build" / "config.build"
    );
    ostringstream stream;

    build2.build("libcrails-odb", true);
    for (const string& backend : OdbModule::supported_backends)
    {
      auto included = find(backends.begin(), backends.end(), backend) != backends.end();

      stream << "\nconfig.libcails_odb.with_" << backend << " = " << (included ? "true" : "false");
    }
    config_build.load_file();
    config_build.append(stream.str());
    config_build.save_file();
    cout << "Supposedly updated " << (build_path / package_name / "build" / "config.build") << " with:\n" << stream.str() << endl << endl;
  }
}

int OdbModule::OdbInstaller::run()
{
  list<string> backends;
  FileRenderer renderer;
  auto         toolchain = configuration.toolchain_editor();

  if (options.count("backends"))
    backends = Crails::split(options["backends"].as<string>(), ',');
  if (!check_backends_validity(backends))
    return -1;
  renderer.vars["task_name"] = string("odb_migrate");
  renderer.vars["project_name"] = configuration.project_name();
  if (!std::filesystem::exists("app/config/databases.cpp"))
  {
    MainCppEditor   main_cpp("exe/server/main.cpp");

    renderer.generate_file("config/databases.hpp", "app/config/databases.hpp");
    renderer.generate_file("config/databases.cpp", "app/config/databases.cpp");
    main_cpp.add_include("config/databases.hpp");
    main_cpp.add_to_main_function("SingletonInstantiator<ApplicationDatabases> databases;\n");
    main_cpp.save_file();
  }
  renderer.generate_file("app/config/odb.hpp");
  renderer.generate_file("exe/odb_migrate/main.cpp");
  if (configuration.toolchain() == "build2")
    renderer.generate_file("scaffolds/task/buildfile", "exe/odb_migrate/buildfile");
  else
    renderer.generate_file("scaffolds/task/CMakeLists.txt", "exe/odb_migrate/CMakeLists.txt");
  configuration.remove_plugin("libcrails-databases"); // must be included after libcrails-odb
  configuration.add_plugin("libcrails-odb");
  configuration.add_plugin("libcrails-databases");
  configuration.add_plugin("libcrails-database-url");
  configuration.variable("odb-backends", Crails::join(backends, ','));
  configuration.variable("odb-at-once", "1");
  configuration.variable("odb-default-pointer", "std::shared_ptr");
  configuration.save();
  toolchain->load_file();
  toolchain->update_plugins();
  toolchain->add_definitions({"WITH_ODB"});
  toolchain->add_dependency("libodb");
  for (const string& backend : backends)
    toolchain->add_dependency("libodb-" + backend);
  toolchain->add_task("odb_migrate");
  toolchain->save_file();
  add_database(configuration, *backends.begin());
  cout << "(i) Don't forget to run `" << configuration.application_build_path() << "/exe/odb_migrate/task -c odb` to create or update your database." << endl;
  return 0;
}

int OdbModule::OdbDisabler::run()
{
  auto toolchain = configuration.toolchain_editor();

  configuration.remove_plugin("libcrails-odb");
  configuration.save();
  toolchain->load_file();
  toolchain->update_plugins();
  toolchain->remove_definitions({"WITH_ODB"});
  toolchain->save_file();
  return 0;
}

int OdbModule::OdbBackendManager::run()
{
  auto toolchain = configuration.toolchain_editor();
  auto backends = Crails::split(configuration.variable("odb-backends"), ',');

  if (!options.count("add") && !options.count("remove"))
    cout << configuration.variable("odb-backends") << endl;
  if (options.count("add"))
  {
    auto new_backends = Crails::split(options["add"].as<string>(), ',');

    toolchain->load_file();
    for (const auto& new_backend : new_backends)
    {
      if (find(backends.begin(), backends.end(), new_backend) == backends.end())
      {
        backends.push_back(new_backend);
        toolchain->add_dependency("libodb-" + new_backend);
      }
    }
    toolchain->save_file();
  }
  if (options.count("remove"))
  {
    auto remove_backends = Crails::split(options["remove"].as<string>(), ',');
    for (const auto& remove_backend : remove_backends)
    {
      auto it = find(backends.begin(), backends.end(), remove_backend);
      if (it != backends.end()) backends.erase(it);
    }
  }
  if (OdbModule::check_backends_validity(backends))
  {
    configuration.variable("odb-backends", Crails::join(backends, ','));
    configuration.save();
    return 0;
  }
  return -1;
}

bool OdbModule::check_backends_validity(const list<string>& backends)
{
  stringstream supported_values;

  supported_values << "Supported values are: " << Crails::join(OdbModule::supported_backends, ' ');
  if (backends.size() == 0)
  {
    cerr << "Must use at least one backend. " << supported_values.str() << endl;
    return false;
  }
  for (const std::string& backend : backends)
  {
    if (std::find(supported_backends.begin(), supported_backends.end(), backend) == supported_backends.end())
    {
      cerr << "Unknown backend `" << backend << "`. " << supported_values.str() << endl;
      return false;
    }
  }
  return true;
}

void render_odb_hpp(const Crails::Renderer&, Crails::RenderTarget& target, Crails::SharedVars&)
{
  target.set_body(std::string(
"#pragma once\n"
"#pragma db model version(1,1)\n"
"#pragma db map type(\"INTEGER\\\\[\\\\]\") as(\"TEXT\") to(\"(?)::INTEGER[]\") from(\"(?)::TEXT\")\n"));
}
